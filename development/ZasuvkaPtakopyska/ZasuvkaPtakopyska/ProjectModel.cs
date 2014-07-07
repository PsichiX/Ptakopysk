using System;
using System.IO;
using System.Xml;
using Newtonsoft.Json;
using System.Collections.Generic;

namespace ZasuvkaPtakopyska
{
    public class ProjectModel
    {
        #region Public Static Data.

        public readonly static int CURRENT_PROJECT_VERSION = 1;

        #endregion



        #region Public Properties.

        public string CbpPath { get; set; }
        public int Version { get; set; }
        [JsonIgnore]
        public string Name { get; set; }
        [JsonIgnore]
        public string WorkingDirectory { get; set; }
        [JsonIgnore]
        public List<string[]> BuildTargets { get; set; }
        public string ActiveTarget { get; set; }
        [JsonIgnore]
        public List<string> Files { get; set; }

        #endregion



        #region Construction and Destruction.

        public ProjectModel(string cbpPath = "")
        {
            CbpPath = cbpPath;
            Version = CURRENT_PROJECT_VERSION;
            Name = "";
            WorkingDirectory = "";
            BuildTargets = new List<string[]>();
            ActiveTarget = "";
            Files = new List<string>();
        }

        #endregion



        #region Public Functionality.

        public void UpdateFromCbp()
        {
            if (String.IsNullOrEmpty(WorkingDirectory))
                WorkingDirectory = "";

            string cbpFilePath = Path.GetFullPath(WorkingDirectory + @"\" + CbpPath);
            if (!File.Exists(cbpFilePath))
                return;

            string xml = File.ReadAllText(cbpFilePath);
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(xml);

            Files = new List<string>();
            XmlNode project = doc.SelectSingleNode("CodeBlocks_project_file/Project");
            foreach (XmlNode node in project.ChildNodes)
            {
                if (node.LocalName == "Option")
                {
                    foreach (XmlAttribute attr in node.Attributes)
                        if (attr.LocalName == "title")
                            Name = attr.Value;
                }
                else if (node.LocalName == "Unit")
                {
                    foreach (XmlAttribute attr in node.Attributes)
                        if (attr.LocalName == "filename")
                            Files.Add(WorkingDirectory + @"\" + attr.Value);
                }
            }
            
            BuildTargets = new List<string[]>();
            XmlNode build = doc.SelectSingleNode("CodeBlocks_project_file/Project/Build");
            foreach (XmlNode node in build.ChildNodes)
            {
                if (node.LocalName == "Target")
                {
                    foreach (XmlAttribute attr in node.Attributes)
                    {
                        if (attr.LocalName == "title")
                        {
                            string output = "";
                            string workingDir = "";
                            foreach (XmlNode _node in node.ChildNodes)
                            {
                                if (_node.LocalName == "Option")
                                {
                                    foreach (XmlAttribute _attr in _node.Attributes)
                                    {
                                        if (_attr.LocalName == "output")
                                            output = _attr.Value;
                                        else if (_attr.LocalName == "working_dir")
                                            workingDir = _attr.Value;
                                    }
                                }
                            }
                            BuildTargets.Add(new string[] { attr.Value, output, workingDir });
                        }
                    }
                }
            }
        }

        public bool ApplyToCbp(SettingsModel settings)
        {
            if (settings == null)
                return false;

            if (String.IsNullOrEmpty(WorkingDirectory))
                WorkingDirectory = "";

            string sdkPath = Path.GetFullPath(settings.SdkPath);
            if (!Directory.Exists(sdkPath))
                return false;
            
            string cbpFilePath = Path.GetFullPath(WorkingDirectory + @"\" + CbpPath);
            if (!File.Exists(cbpFilePath))
                return false;

            string xml = File.ReadAllText(cbpFilePath);
            XmlDocument doc = new XmlDocument();
            doc.LoadXml(xml);
            XmlNode compiler = doc.SelectSingleNode("CodeBlocks_project_file/Project/Compiler");
            foreach (XmlNode node in compiler.ChildNodes)
                if (node.LocalName == "Add")
                    foreach (XmlAttribute attr in node.Attributes)
                        if (attr.LocalName == "directory")
                            attr.Value = sdkPath + @"\include";
            
            XmlNode linker = doc.SelectSingleNode("CodeBlocks_project_file/Project/Linker");
            foreach (XmlNode node in linker.ChildNodes)
                if (node.LocalName == "Add")
                    foreach (XmlAttribute attr in node.Attributes)
                        if (attr.LocalName == "directory")
                            attr.Value = sdkPath + @"\lib";
            
            doc.Save(cbpFilePath);
            return true;
        }

        #endregion
    }
}
