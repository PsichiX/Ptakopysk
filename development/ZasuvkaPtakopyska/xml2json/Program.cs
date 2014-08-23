using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;

namespace xml2json
{
    class Program
    {
        /// <summary>
        /// Convert between Xml and Json files.
        /// </summary>
        /// <param name="args">
        /// xml2json.exe inputFilePath outputFilePath
        /// </param>
        static void Main(string[] args)
        {
            Console.WriteLine("> File converter betwen Xml and Json");

            for (int i = 0; i < args.Length; ++i)
                args[i] = args[i].Replace("\"", "").Replace("'", "");

            if (args != null && args.Length > 1)
            {
                string input = args[0];
                string output = args[1];

                if (File.Exists(input))
                    ProcessFile(input, output);
                else
                    Console.WriteLine("Input file does not exists: \"" + input + "\"!");
            }
            else
            {
                Console.WriteLine(@"# Example of usage:");
                Console.WriteLine(@"# xml2json.exe Path\to\input\file Path\to\output\file");
            }
        }

        private static void ProcessFile(string input, string output)
        {
            if (Path.GetExtension(input) == ".xml" && Path.GetExtension(output) == ".json")
            {
                XmlDocument xml = new XmlDocument();
                xml.Load(input);
                string json = Newtonsoft.Json.JsonConvert.SerializeXmlNode(xml, Newtonsoft.Json.Formatting.Indented);
                if (!String.IsNullOrEmpty(json))
                {
                    File.WriteAllText(output, json);
                    Console.WriteLine("File converted!");
                }
                else
                    Console.WriteLine("Error during converting from: \"" + Path.GetExtension(input) + "\" to: \"" + Path.GetExtension(output) + "\"!");
            }
            else if (Path.GetExtension(input) == ".json" && Path.GetExtension(output) == ".xml")
            {
                string json = File.ReadAllText(input);
                XmlDocument xml = Newtonsoft.Json.JsonConvert.DeserializeXmlNode(json);
                if (xml != null)
                {
                    xml.Save(output);
                    Console.WriteLine("File converted!");
                }
                else
                    Console.WriteLine("Error during converting from: \"" + Path.GetExtension(input) + "\" to: \"" + Path.GetExtension(output) + "\"!");
            }
            else
                Console.WriteLine("Cannot convert from: \"" + Path.GetExtension(input) + "\" to: \"" + Path.GetExtension(output) + "\"!");
        }
    }
}
