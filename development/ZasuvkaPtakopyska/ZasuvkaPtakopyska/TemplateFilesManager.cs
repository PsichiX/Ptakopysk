using System;
using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;
using Newtonsoft.Json.Linq;
using ZasuvkaPtakopyskaExtender;

namespace ZasuvkaPtakopyska
{
    public class TemplateFilesManager
    {
        public static bool ProcessTemplates(string file, JObject args)
        {
            if (!File.Exists(file))
                return false;

            Dictionary<string, string> a = new Dictionary<string, string>();
            if (args != null)
                foreach (var kv in args)
                    if (kv.Value.Type == JTokenType.String)
                        a[kv.Key] = kv.Value.ToObject<string>();

            string content = ReplaceArguments(File.ReadAllText(file), a);
            try
            {
                object root = Newtonsoft.Json.JsonConvert.DeserializeObject(content);
                JArray commands = root as JArray;
                if (root != null)
                {
                    bool somethingChanged = false;
                    JArray cmd;
                    string cmdName;
                    foreach (var command in commands)
                    {
                        cmd = command as JArray;
                        if (cmd != null && cmd.Count > 0 && cmd[0].Type == JTokenType.String)
                        {
                            cmdName = cmd[0].ToObject<string>();
                            if (cmdName == "copyFile" && cmd.Count == 3 && cmd[1].Type == JTokenType.String && cmd[2].Type == JTokenType.String)
                            {
                                string pathFrom = cmd[1].ToObject<string>();
                                string pathTo = cmd[2].ToObject<string>();
                                if (File.Exists(pathFrom))
                                {
                                    File.Copy(pathFrom, pathTo, true);
                                    somethingChanged = true;
                                }
                            }
                            else if (cmdName == "copyDir" && cmd.Count == 3 && cmd[1].Type == JTokenType.String && cmd[2].Type == JTokenType.String)
                            {
                                string pathFrom = cmd[1].ToObject<string>();
                                string pathTo = cmd[2].ToObject<string>();
                                if (Directory.Exists(pathFrom))
                                {
                                    DirectoryCopy(pathFrom, pathTo);
                                    somethingChanged = true;
                                }
                            }
                            else if (cmdName == "makeDir" && cmd.Count == 2 && cmd[1].Type == JTokenType.String)
                            {
                                string path = cmd[1].ToObject<string>();
                                if (!Directory.Exists(path))
                                {
                                    Directory.CreateDirectory(path);
                                    somethingChanged = true;
                                }
                            }
                            else if (cmdName == "replaceInFile" && cmd.Count == 3 && cmd[1].Type == JTokenType.String && cmd[2].Type == JTokenType.Array)
                            {
                                string path = cmd[1].ToObject<string>();
                                JArray rules = cmd[2] as JArray;
                                if (File.Exists(path) && rules.Count > 0)
                                {
                                    string _content = File.ReadAllText(path);
                                    JArray item;
                                    string pattern;
                                    string replacement;
                                    foreach (var rule in rules)
                                    {
                                        item = rule as JArray;
                                        if (item != null && item.Count == 2 && item[0].Type == JTokenType.String && item[1].Type == JTokenType.String)
                                        {
                                            pattern = item[0].ToObject<string>();
                                            replacement = item[1].ToObject<string>();
                                            Regex rgx = new Regex(pattern);
                                            _content = rgx.Replace(_content, replacement);
                                        }
                                    }
                                    File.WriteAllText(path, _content);
                                    somethingChanged = true;
                                }
                            }
                            else if (cmdName == "processTemplates" && cmd.Count == 3 && cmd[1].Type == JTokenType.String && cmd[2].Type == JTokenType.Object)
                            {
                                string path = cmd[1].ToObject<string>();
                                if (!Path.IsPathRooted(path))
                                    path = Path.GetDirectoryName(file) + @"\" + path;
                                JObject _args = cmd[2] as JObject;
                                if (ProcessTemplates(path, _args))
                                    somethingChanged = true;
                            }
                        }
                    }
                    return somethingChanged;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine(">>> " + ex.GetType().ToString() + ": " + ex.Message);
            }
            return false;
        }

        private static string ReplaceArguments(string input, Dictionary<string, string> args)
        {
            if (!string.IsNullOrEmpty(input) && args != null && args.Count > 0)
                foreach (var kv in args)
                    input = input.Replace("@" + kv.Key + "@", Utils.EscapedString.Escape(kv.Value));
            return input;
        }

        private static void DirectoryCopy(string sourceDirName, string destDirName, bool copySubDirs = true)
        {
            DirectoryInfo dir = new DirectoryInfo(sourceDirName);
            DirectoryInfo[] dirs = dir.GetDirectories();

            if (!dir.Exists)
                throw new DirectoryNotFoundException("Source directory does not exist or could not be found: " + sourceDirName);

            if (!Directory.Exists(destDirName))
                Directory.CreateDirectory(destDirName);

            FileInfo[] files = dir.GetFiles();
            foreach (FileInfo file in files)
            {
                string temppath = Path.Combine(destDirName, file.Name);
                file.CopyTo(temppath, false);
            }

            if (copySubDirs)
            {
                foreach (DirectoryInfo subdir in dirs)
                {
                    string temppath = Path.Combine(destDirName, subdir.Name);
                    DirectoryCopy(subdir.FullName, temppath, copySubDirs);
                }
            }
        }
    }
}
