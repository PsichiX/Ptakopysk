using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Diagnostics;
using IntuicioIdePlugin;
using ICSharpCode.TextEditor;
using ICSharpCode.TextEditor.Document;

namespace TimusTinyBasicPlugin
{
    /// <summary>
    /// Plugin class that implements IIdePlugin interface.
    /// Must provide PluginName attribute so IDE will be able to recognize this plugin by it's name.
    /// </summary>
    [PluginName(Constants.NAME)]
    public class TimusTinyBasicPlugin : IIdePlugin
    {
        /// <summary>
        /// Object that implements IIdeBridge interface to communicate with IDE from plugin.
        /// </summary>
        private IIdeBridge mBridge;

        /// <summary>
        /// Called when plugin is loaded into IDE.
        /// </summary>
        /// <param name="bridge">Object used to communicate with IDE from plugin.</param>
        public void onLoad(IIdeBridge bridge)
        {
            mBridge = bridge;
            FileSyntaxModeProvider fsmp = new FileSyntaxModeProvider(mBridge.GetPluginsPath() + @"\syntax");
            HighlightingManager.Manager.AddSyntaxModeFileProvider(fsmp);
        }

        /// <summary>
        /// Called when plugin is unloaded from IDE.
        /// Here you should cleanup anything.
        /// </summary>
        public void onUnload()
        {
            mBridge = null;
        }

        /// <summary>
        /// Called when user performs Plugin Action from IDE menu.
        /// </summary>
        /// <param name="actionTag">Action tag defined in GetActionMenuList() method.</param>
        public void onAction(String actionTag)
        {
            // "documentation" action tag.
            if (actionTag.Equals("documentation"))
            {
                // show Tiny BASIC online documentation inside web browser.
                WebBrowser wb = new WebBrowser();
                wb.Dock = DockStyle.Fill;
                mBridge.onOpenUnmanagedDocument(this, wb, "Tiny BASIC Documentation");
                wb.Navigate(new Uri("http://www.ittybittycomputers.com/IttyBitty/TinyBasic/TBuserMan.htm"));
            }
        }

        /// <summary>
        /// Called when new project is created.
        /// Here just create and fill project descriptor.
        /// </summary>
        /// <param name="name">Project name.</param>
        /// <param name="dir">Project location (directory path)</param>
        /// <returns></returns>
        public IdeProject onNewProject(String name, String dir)
        {
            // create new project descriptor and fill it with default data.
            IdeProject proj = new IdeProject();
            proj.type = Constants.NAME; // plugin name as project type.
            proj.name = name;
            proj.outputDir = "bin"; // compiled binaries output directory.
            return proj;
        }

        /// <summary>
        /// Called after project creation.
        /// Here you should initialize your project content.
        /// </summary>
        /// <param name="project">Project descriptor.</param>
        /// <returns>True if successful.</returns>
        public bool onProjectCreated(IdeProject project)
        {
            // create main project file.
            IdeProject.File f = new IdeProject.File();
            // file uppercased extension as file type. It can be anything, but that way it's simple to manage.
            f.type = "BAS";
            // file path relative to project directory.
            f.relativePath = "main.bas";
            // add created file to project files list.
            project.files.Add(f);
            // then notify IDE (using bridge) to update views with new project content.
            mBridge.onProjectChanged(project);
            return true;
        }

        /// <summary>
        /// Called when project exists and it was loaded into IDE.
        /// </summary>
        /// <param name="project">Project descriptor.</param>
        /// <returns>True if successful.</returns>
        public bool onLoadProject(IdeProject project)
        {
            return true;
        }

        /// <summary>
        /// Called when project is saved.
        /// </summary>
        /// <param name="project">Project descriptor.</param>
        /// <returns>True if succesful.</returns>
        public bool onSaveProject(IdeProject project)
        {
            return true;
        }

        /// <summary>
        /// Called when new file was created and added to project.
        /// </summary>
        /// <param name="file">File descriptor.</param>
        /// <param name="exists">True if file already exists in file system (just adding, without creating).</param>
        /// <returns>Form control as IDE document content.</returns>
        public Control onNewFile(IdeProject.File file, bool exists)
        {
            // create text editor control that will be able to edit our scripts.
            TextEditorControl editor = new TextEditorControl();
            editor.Dock = DockStyle.Fill;
            editor.IsReadOnly = false;
            // here we can inject syntax highlighting for Tiny BASIC language.
            try
            {
                editor.SetHighlighting("TinyBASIC");
            }
            catch (Exception exc)
            {
                while (exc.InnerException != null)
                    exc = exc.InnerException;
                MessageBox.Show(@"Error occurred during Syntax Highlight binding to code editor:" + Environment.NewLine + exc.Message, Constants.NAME + @" Plugin Exception");
            }
            editor.Encoding = Encoding.ASCII; // make sure that we wants to use ASCII encoding.
            if (!exists)
            {
                // setup editor content and save it to file if file does not exists.
                editor.Text = Constants.FILE_TEMPLATE;
                editor.SaveFile(mBridge.GetProjectPath() + @"\" + file.relativePath);
            }
            else
                // or load file to editor otherwise.
                editor.LoadFile(mBridge.GetProjectPath() + @"\" + file.relativePath);
            editor.Encoding = Encoding.ASCII;
            return editor;
        }

        /// <summary>
        /// Called when project file is loaded into IDE as document.
        /// </summary>
        /// <param name="file">File descriptor.</param>
        /// <returns>Form control as IDE document content.</returns>
        public Control onLoadFile(IdeProject.File file)
        {
            TextEditorControl editor = new TextEditorControl();
            editor.Dock = DockStyle.Fill;
            editor.IsReadOnly = false;
            try
            {
                editor.SetHighlighting("TinyBASIC");
            }
            catch (Exception exc)
            {
                while (exc.InnerException != null)
                    exc = exc.InnerException;
                MessageBox.Show(@"Error occurred during Syntax Highlight binding to code editor:" + Environment.NewLine + exc.Message, Constants.NAME + @" Plugin Exception");
            }
            editor.Encoding = Encoding.ASCII;
            editor.LoadFile(mBridge.GetProjectPath() + @"\" + file.relativePath);
            editor.Encoding = Encoding.ASCII;
            return editor;
        }

        /// <summary>
        /// Called when project file is saved.
        /// </summary>
        /// <param name="file">File descriptor.</param>
        /// <param name="control">IDE document content control.</param>
        /// <returns>Form control as IDE document content.</returns>
        public bool onSaveFile(IdeProject.File file, Control control)
        {
            if (!(control is TextEditorControl))
                return false;
            TextEditorControl editor = control as TextEditorControl;
            editor.Encoding = Encoding.ASCII;
            editor.SaveFile(mBridge.GetProjectPath() + @"\" + file.relativePath);
            editor.Encoding = Encoding.ASCII;
            return true;
        }

        /// <summary>
        /// Compile Tiny BASIC script to Intuicio Assembly script.
        /// </summary>
        /// <param name="input">File path to input Tiny BASIC script.</param>
        /// <param name="output">File path to output Intuicio Assembly script.</param>
        /// <returns></returns>
        private bool CompileTinyBasic(String input, String output)
        {
            // make sure that input file exists.
            if (!File.Exists(input))
                return false;
            // prepare arguments for Tiny BASIC compiler.
            String args = "-o \"" + output + "\" \"" + input + "\"";
            // prepare and run Tiny BASIC compiler.
            ProcessStartInfo psi = new ProcessStartInfo(mBridge.GetPluginsPath() + @"\ttbc.exe", args);
            psi.UseShellExecute = false;
            psi.RedirectStandardOutput = true;
            psi.RedirectStandardError = true;
            psi.CreateNoWindow = true;
            mBridge.onLogInfo(@"Tiny BASIC compilation start for file: '" + Path.GetFileName(input) + "'" + Environment.NewLine + "Tiny BASIC compiler arguments: " + args + Environment.NewLine);
            Process proc = Process.Start(psi);
            proc.WaitForExit();
            if (proc.ExitCode == 0)
            {
                while (!proc.StandardOutput.EndOfStream)
                    mBridge.onLogInfo(proc.StandardOutput.ReadLine() + Environment.NewLine);
                mBridge.onLogInfo(@"Successful Tiny BASIC compilation for file: '" + Path.GetFileName(input) + "'" + Environment.NewLine);
                return true;
            }
            else
            {
                while (!proc.StandardError.EndOfStream)
                    mBridge.onLogError(proc.StandardError.ReadLine() + Environment.NewLine);
                mBridge.onLogError(@"Tiny BASIC compilation failed for file: '" + Path.GetFileName(input) + "'" + Environment.NewLine);
                return false;
            }
        }

        /// <summary>
        /// Called when IDE performs project compilation.
        /// </summary>
        /// <param name="project">Project descriptor.</param>
        /// <returns>True if successful.</returns>
        public bool onBuildProject(IdeProject project)
        {
            String projDir = mBridge.GetProjectPath();
            if (String.IsNullOrEmpty(projDir))
                return false;
            // iterate over all project script files.
            foreach (IdeProject.File file in project.files)
            {
                // skip script if is not compilable.
                if (!file.compile)
                    continue;
                // prepare source Tiny BASIC file path.
                String srcPath = Path.GetFullPath(projDir + @"\" + file.relativePath);
                // make source path relative to project directory.
                String relPath = Utils.MakeRelativePath(projDir + @"\", srcPath);
                // prepare destination Intuicio Assembly script path.
                String destIscPath = Path.ChangeExtension(Path.GetFullPath(projDir + @"\" + project.outputDir + @"\" + relPath), "isc");
                // prepare destination immediate (precompiled) Intuicio Assembly script path.
                String destImmPath = Path.ChangeExtension(Path.GetFullPath(projDir + @"\" + project.outputDir + @"\" + relPath), "imm");
                // prepare destination Intuicio Program path.
                String destItcPath = Path.ChangeExtension(Path.GetFullPath(projDir + @"\" + project.outputDir + @"\" + relPath), "itc");
                // make sure that destination directory exists.
                Directory.CreateDirectory(Path.GetDirectoryName(destItcPath));
                // compile Tiny BASIC script to Intuicio Assembly script.
                if (!CompileTinyBasic(srcPath, destIscPath))
                    return false;
                // compile Intuicio Assembly script to Intuicio Program.
                if (!mBridge.CompileIntuicioAssembly(
                    destIscPath, // source script path.
                    destItcPath, // destination program path.
                    destImmPath, // immediate file path.
                    new String[]{
                        mBridge.GetSdksPath() // here you can add another search path.
                    }))
                    return false;
            }
            return true;
        }

        /// <summary>
        /// Gets array of binaries file names.
        /// </summary>
        /// <param name="dir">Directory where you searching for programs.</param>
        /// <param name="pattern">File name pattern.</param>
        /// <returns></returns>
        private String[] GetListOfBinaries(String dir, String pattern)
        {
            List<String> bins = new List<String>();
            if (Directory.Exists(dir))
            {
                String[] files = Directory.GetFiles(dir, pattern);
                if (files != null)
                    foreach (String file in files)
                        bins.Add(file);
                String[] sdirs = Directory.GetDirectories(dir);
                if (sdirs != null)
                    foreach (String sdir in sdirs)
                        bins.AddRange(GetListOfBinaries(sdir, pattern));
            }
            return bins.Count > 0 ? bins.ToArray<String>() : null;
        }

        /// <summary>
        /// Called when IDE performs running of compiled programs.
        /// </summary>
        /// <param name="project">Project descriptor.</param>
        /// <returns>True if successful.</returns>
        public bool onRunProject(IdeProject project)
        {
            String projDir = mBridge.GetProjectPath();
            if (String.IsNullOrEmpty(projDir))
                return false;
            String binDir = Path.GetFullPath(projDir + @"\" + project.outputDir);
            if (!Directory.Exists(binDir))
                return false;
            String[] bins = GetListOfBinaries(binDir, "*.itc");
            String bin = bins.Length > 0 ? (
                bins.Length == 1 ? bins[0] : mBridge.onSelectProgram(bins) // if there is only one compiled program, run it, otherwise use binary selection dialog from IDE.
                ) : null;
            if (!String.IsNullOrEmpty(bin))
                return mBridge.RunIntuicioProgram(bin, true); // run Intuicio Program if found.
            return false;
        }

        /// <summary>
        /// Called when IDE is closing unmanaged document opened by this plugin.
        /// </summary>
        /// <param name="control">Document content control.</param>
        /// <returns>True if successful.</returns>
        public bool onCloseUnmanagedDocument(Control control)
        {
            return true;
        }

        /// <summary>
        /// Get default Tiny BASIC file extension.
        /// </summary>
        /// <returns>Default Tiny BASIC file extension.</returns>
        public String GetDefaultExt()
        {
            return @"bas";
        }

        /// <summary>
        /// Get file filters used by this plugin.
        /// </summary>
        /// <returns>File filters.</returns>
        public String GetFileFilters()
        {
            return @"Tiny BASIC script (*.bas)|*.bas";
        }

        /// <summary>
        /// Get plugin usage flags to notify IDE what this plugin can do.
        /// </summary>
        /// <returns>Plugin usage flags.</returns>
        public Utils.PluginUsageFlags GetUsageFlags()
        {
            return Utils.PluginUsageFlags.All;
        }

        /// <summary>
        /// Get plugins names required by this plugin.
        /// </summary>
        /// <returns>Required plugins names array or null if not required.</returns>
        public String[] GetRequiredPlugins()
        {
            return null;
        }

        /// <summary>
        /// Get dictionary of "Action name in menu"=>"action_tag" content.
        /// </summary>
        /// <returns>Dictionary with plugin actions.</returns>
        public Dictionary<String, String> GetActionMenuList()
        {
            Dictionary<String, String> dict = new Dictionary<string, string>();
            dict["Open documentation"] = "documentation";
            return dict;
        }
    }
}
