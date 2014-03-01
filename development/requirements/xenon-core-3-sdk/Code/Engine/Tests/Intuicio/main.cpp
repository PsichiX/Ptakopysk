#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <map>
#include <XeCore/Common/Buffer.h>
#include <XeCore/Common/String.h>
#include <XeCore/Intuicio/IntuicioVM.h>
#include <XeCore/Intuicio/ContextVM.h>
#include <IntuicioCompiler.h>

#define SUCCESS { system( "pause"); return EXIT_SUCCESS; }
#define FAILURE { system( "pause"); return EXIT_FAILURE; }

using namespace XeCore::Common;
using namespace XeCore::Intuicio;
using namespace std;

///
/// Interceptions context support.
///
class Interceptions : public ContextVM::OnInterceptListener
{
public:
    /// Constructor.
    Interceptions( ContextVM* context )
    {
        m_context = context;
    }

    /// Destructor.
    ~Interceptions()
    {
        m_context = 0;
    }

    /// Called when VM performs interception.
    /// While onIntercept() is called, VM waits for interception operation result.
    /// Return true if any error occurred, false if operation was successful.
    bool onIntercept( ParallelThreadVM* caller, unsigned int code )
    {
        /// Calculate powered value from VM and return result to VM using stack.
        if( code == 1 )
        {
            int val = 0;
            m_context->stackPop( caller, &val, sizeof( int ) );
            val *= val;
            m_context->stackPush( caller, &val, sizeof( int ) );
            return false;
        }
        /// Get integer value from input console and return it to VM using stack.
        else if( code == 2 )
        {
            int val = 0;
            cin >> val;
            m_context->stackPush( caller, &val, sizeof( int ) );
            return false;
        }
        return true;
    }

private:
    ContextVM*  m_context;
};

///
/// Program entry point.
///
int main()
{
    /// array with scripts filenames.
    const char* fileNames[] = {
        "test_arithmetics.isc",
        "test_jumps.isc",
        "test_calls.isc",
        "test_external_data.isc",
        "test_dynamic_allocation.isc",
        "test_structs.isc",
        "test_preprocessor.isc",
        "test_namespaces.isc",
        "test_interceptions.isc",
        "test_managed_memory.isc"
    };

    /// make map for external data to bind with VM context.
    int extNum0 = 5;
    int extNum1 = 2;
    std::map< String, void* > externals;
    externals[ "extNum0" ] = &extNum0;
    externals[ "extNum1" ] = &extNum1;

    /// create Intuicio VM instance.
    IntuicioVM* vm = xnew IntuicioVM();
    if( !vm )
    {
        cerr << "Cannot create Intuicio Virtual Machine!" << endl;
        FAILURE;
    }

    /// select script to compile and run.
    cout << "Select program to run:" << endl;
    cout << "0. EXIT" << endl;
    cout << "1. Arithmetics" << endl;
    cout << "2. Conditional jumps" << endl;
    cout << "3. Functon calls" << endl;
    cout << "4. External data" << endl;
    cout << "5. Dynamic allocation" << endl;
    cout << "6. Structures" << endl;
    cout << "7. Preprocessor" << endl;
    cout << "8. Namespaces" << endl;
    cout << "9. Interceptions" << endl;
    cout << "10. Managed memory" << endl;
    cout << "Type program id: ";

    unsigned int option = 0;
    cin >> option;
    if( option < 1 || option > sizeof( fileNames ) / sizeof( char* ) )
    {
        DELETE_OBJECT( vm );
        FAILURE;
    }
    option -= 1;

    /// prepare input file name.
    stringstream snames;
    snames << fileNames[ option ];
    String fname( snames.str() );
    snames.str( "" );

    /// prepare output file name.
    snames << "programs/" << fileNames[ option ] << ".itc";
    String ifname( snames.str() );
    snames.str( "" );

    /// prepare immediate file name.
    snames << "programs/" << fileNames[ option ] << ".imm";
    String mfname( snames.str() );
    snames.str( "" );

    /// compile script.
    stringstream cmdargs;
    cmdargs << "-i " << fname << " -o " << ifname << " -m " << mfname;
    String errors;
    if( !compile( cmdargs.str(), errors ) )
    {
        cerr << errors << endl;
        cerr << "Compilation failed!" << endl;
        DELETE_OBJECT( vm );
        FAILURE;
    }
    else
        cout << errors << endl;

    /// open compiled Intuicio Program:
    ifstream file( ifname.c_str(), ifstream::in | ifstream::binary );
    if( !file )
    {
        cerr << "Cannot open program file: '" << ifname << "'!" << endl;
        DELETE_OBJECT( vm );
        FAILURE;
    }
    /// make VM program object from opened file.
    file.seekg( 0, ifstream::end );
    unsigned int fsize = file.tellg();
    cout << "Program size: " << fsize << " bytes" << endl;
    file.seekg( 0, ifstream::beg );
    ProgramVM* prog = xnew ProgramVM();
    prog->getBuffer()->resize( fsize );
    file.read( (char*)prog->getBuffer()->data(), fsize );
    prog->getBuffer()->reposition();
    file.close();

    /// validate created program.
    if( !prog->validate() )
    {
        cerr << "Program is not valid: '" << ifname << "'!" << endl;
        DELETE_OBJECT( vm );
        DELETE_OBJECT( prog );
        FAILURE;
    }
    cout << "Successful compiled program!" << endl;

    cout << "Exports:\n";
    map< String, ProgramVM::Pointer > exports;
    prog->getExports( exports );
    unsigned int startAt = 0;
    for( map< String, ProgramVM::Pointer >::iterator it = exports.begin(); it != exports.end(); it++ )
    {
        cout << "[" << it->first << "] location: " << it->second.location << "; address: " << it->second.address << endl;
        if( it->first == "entrypoint" )
            startAt = it->second.address;
    }

    /// create VM context to run loaded program.
    ContextVM* cntx = vm->createContext( prog );
    if( cntx )
    {
        /// create and register interceptions listener.
        Interceptions* listener = xnew Interceptions( cntx );
        if( cntx->registerInterceptionListener( "NativeInterceptions", listener ) )
        {
            /// bind external data map.
            if( cntx->bindExternals( externals ) )
            {
                /// run program and wait to it's end (as default program runs in separated thread).
                cout << "Running program:" << endl << endl;
                cntx->runProgram( true, startAt );
                /// unbind external data map.
                cntx->unbindExternals();
                cout << endl;
            }
            else
            {
                cerr << "Some externals has not been provided!" << endl;
                DELETE_OBJECT( listener );
                DELETE_OBJECT( vm );
                DELETE_OBJECT( prog );
                FAILURE;
            }
        }
        else
        {
            cerr << "Cannot register interceptions listener!" << endl;
            DELETE_OBJECT( listener );
            DELETE_OBJECT( vm );
            DELETE_OBJECT( prog );
            FAILURE;
        }
        DELETE_OBJECT( listener );
    }
    else
    {
        cerr << "Cannot create program context!" << endl;
        DELETE_OBJECT( vm );
        DELETE_OBJECT( prog );
        FAILURE;
    }

    DELETE_OBJECT( vm );
    DELETE_OBJECT( prog );
    SUCCESS;
}
