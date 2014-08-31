using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using PtakopyskMetaGenerator;

namespace ConsoleMetaGenerator
{
    class Program
    {
        /// <summary>
        /// Generates meta-files from cpp header files.
        /// </summary>
        /// <param name="args">
        /// ConsoleMetaGenerator.exe -type inputDirectoryOrFilePath [outputDirectoryOrFilePath]
        /// </param>
        static void Main(string[] args)
        {
            Console.WriteLine("> Ptakopysk Meta File Generator");

            for (int i = 0; i < args.Length; ++i)
                args[i] = args[i].Replace("\"", "").Replace("'", "");

            if (args != null && args.Length > 1)
            {
                string type = args[0];
                string input = args[1];
                string output = args.Length > 2 ? args[2] : null;
                
                if (File.Exists(input))
                    ProcessFile(input, output, type);
                else if (Directory.Exists(input))
                    ProcessDirectory(input, output, type);
            }
            else
            {
                Console.WriteLine(@"# Example of usage:");
                Console.WriteLine(@"# ConsoleMetaGenerator.exe -MetaType Path\to\input\file\or\directory Path\to\output\file\or\directory");
            }
        }

        private static void ProcessFile(string input, string output, string type)
        {
            if (string.IsNullOrEmpty(input))
                return;
            input = Path.GetFullPath(input);
            if (string.IsNullOrEmpty(output))
                output = input;
            //if (!Path.IsPathRooted(output))
            //    output = Path.GetDirectoryName(input) + @"\" + output;

            if (Path.GetExtension(output) != ".meta")
                output += ".meta";

            string content = File.ReadAllText(input);
            string log = "";
            string json = null;
            if (type == "-component" || type == "-c")
                json = MetaCpp.GenerateMetaComponentJson(content, out log);
            else if (type == "-asset" || type == "-a")
                json = MetaCpp.GenerateMetaAssetJson(content, out log);
            if (json != null)
            {
                Console.WriteLine();
                Console.WriteLine("Input: " + input);
                Console.WriteLine("Output: " + output);
                File.WriteAllText(output, json);
            }
        }

        private static void ProcessDirectory(string input, string output, string type)
        {
            DirectoryInfo dir = new DirectoryInfo(input);
            if (!dir.Exists)
                return;

            foreach (FileInfo info in dir.GetFiles("*.h"))
                ProcessFile(input + @"\" + info.Name, output, type);
            foreach (DirectoryInfo info in dir.GetDirectories())
                ProcessDirectory(input + @"\" + info.Name, output, type);
        }
    }
}
