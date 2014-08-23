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
        /// ConsoleMetaGenerator.exe inputDirectoryOrFilePath [outputDirectoryOrFilePath]
        /// </param>
        static void Main(string[] args)
        {
            Console.WriteLine("> Ptakopysk Meta File Generator");

            for (int i = 0; i < args.Length; ++i)
                args[i] = args[i].Replace("\"", "").Replace("'", "");

            if (args != null && args.Length > 0)
            {
                string input = args[0];
                string output = args.Length > 1 ? args[1] : "";

                if (File.Exists(input))
                    ProcessFile(input, output);
                else if (Directory.Exists(input))
                    ProcessDirectory(input, output);
            }
            else
            {
                Console.WriteLine(@"# Example of usage:");
                Console.WriteLine(@"# ConsoleMetaGenerator.exe Path\to\input\file\or\directory [Path\to\output\file\or\directory]");
            }
        }

        private static void ProcessFile(string input, string output)
        {
            if (String.IsNullOrEmpty(output))
                output = input;
            else if (!Path.IsPathRooted(output))
                output = Path.GetDirectoryName(input) + @"\" + output;

            if (Path.GetExtension(output) != ".meta")
                output += ".meta";

            string content = File.ReadAllText(input);
            string log = "";
            string json = MetaCpp.GenerateMetaComponentJson(content, out log);
            if (json != null)
            {
                Console.WriteLine();
                Console.WriteLine("Input: " + input);
                Console.WriteLine("Output: " + output);
                File.WriteAllText(output, json);
            }
        }

        private static void ProcessDirectory(string input, string output)
        {
            DirectoryInfo dir = new DirectoryInfo(input);
            foreach (FileInfo info in dir.GetFiles("*.h"))
                ProcessFile(input + @"\" + info.Name, output);
            foreach (DirectoryInfo info in dir.GetDirectories())
                ProcessDirectory(input + @"\" + info.Name, output);
        }
    }
}
