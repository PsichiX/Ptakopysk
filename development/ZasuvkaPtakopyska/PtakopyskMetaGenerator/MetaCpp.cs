using System.Collections.Generic;
using CppRipper;

namespace PtakopyskMetaGenerator
{
    public class MetaCpp
    {
        private static readonly string META_COMPONENT = "META_COMPONENT";
        private static readonly string META_PROPERTY = "META_PROPERTY";
        private static readonly string META_ATTR_NAME = "META_ATTR_NAME";
        private static readonly string META_ATTR_VALUE_TYPE = "META_ATTR_VALUE_TYPE";
        private static readonly string META_ATTR_DESCRIPTION = "META_ATTR_DESCRIPTION";
        private static readonly string META_ATTR_DEFAULT_VALUE = "META_ATTR_DEFAULT_VALUE";
        private static readonly string XECORE_COMMON_PROPERTY = "XeCore::Common::Property";

        private static readonly string RULE_NAME_UNNAMED = "_unnamed_";
        private static readonly string RULE_NAME_PARAN_GROUP = "paran_group";
        private static readonly string RULE_NAME_BRACE_GROUP = "brace_group";
        private static readonly string RULE_NAME_CLASS_DECL = "class_decl";
        private static readonly string RULE_NAME_DECLARATION_CONTENT = "declaration_content";
        private static readonly string RULE_NAME_DECLARATION_LIST = "declaration_list";
        private static readonly string RULE_NAME_CLASS = "CLASS";
        private static readonly string RULE_NAME_IDENTIFIER = "identifier";
        private static readonly string RULE_NAME_STRING_LITERAL = "string_literal";
        private static readonly string RULE_TYPE_STAR = "star";
        private static readonly string RULE_TYPE_CHARSET = "charset";

        public static string GenerateMetaComponentJson(string cppContent, out string status)
        {
            MetaComponent meta = GenerateMetaComponent(cppContent, out status);
            return meta == null ? null : Newtonsoft.Json.JsonConvert.SerializeObject(meta, Newtonsoft.Json.Formatting.Indented);
        }

        public static MetaComponent GenerateMetaComponent(string cppContent, out string status)
        {
            ParseNode root = GenerateCppAst(cppContent, out status);
            return root == null ? null : FindMetaComponent(root);
        }

        public static string GenerateCppAstJson(string cppContent, out string status)
        {
            ParseNode root = GenerateCppAst(cppContent, out status);
            return root == null ? null : Newtonsoft.Json.JsonConvert.SerializeObject(root, Newtonsoft.Json.Formatting.Indented);
        }

        public static CppRipper.ParseNode GenerateCppAst(string cppContent, out string status)
        {
            CppStructuralGrammar grammar = new CppStructuralGrammar();
            Rule parse_rule = grammar.file;
            ParserState state = new ParserState(cppContent);

            try
            {
                if (!parse_rule.Match(state))
                {
                    status = "Failed to parse c++ content";
                    return null;
                }
                else
                {
                    if (state.AtEndOfInput())
                    {
                        status = "Successfully parsed c++ content";
                        return state.GetRoot();
                    }
                    else
                    {
                        status = "Failed to read end of c++ content";
                        return null;
                    }
                }
            }
            catch (ParsingException e)
            {
                state.ForceCompletion();
                status = e.Message;
                return null;
            }
        }

        public static MetaComponent FindMetaComponent(ParseNode node)
        {
            return node == null ? null : SearchDownForMetaComponent(node);
        }

        private static MetaComponent SearchDownForMetaComponent(ParseNode node)
        {
            if (node == null)
                return null;

            MetaComponent meta;
            if (node.RuleName == RULE_NAME_CLASS_DECL && node.Count >= 2 && node[0].RuleName == RULE_NAME_CLASS)
            {
                ParseNode className = SearchDownFor(node[1], RULE_NAME_IDENTIFIER);
                if (className != null)
                {
                    ParseNode parent = SearchUpForDeclarationContent(node);
                    if (parent != null)
                    {
                        int i = 0;
                        while (i < parent.Count && !HasRuleWithValue(parent[i], RULE_NAME_IDENTIFIER, META_COMPONENT)) ++i;
                        ++i;
                        if (i < parent.Count)
                        {
                            meta = new MetaComponent(className.Value);
                            Dictionary<string, string> attr = SearchDownForAttributes(parent[i]);
                            if (attr != null)
                            {
                                foreach (string key in attr.Keys)
                                {
                                    if (key == META_ATTR_NAME)
                                        meta.Name = attr[key];
                                    else if (key == META_ATTR_DESCRIPTION)
                                        meta.Description = attr[key];
                                }
                            }
                            meta.Properties = SearchDownForProperties(parent);
                            i += 2;
                            if (i < parent.Count - 1 && HasRuleWithValue(parent[i], RULE_NAME_UNNAMED, ":", RULE_TYPE_CHARSET))
                            {
                                List<string> baseClasses = new List<string>();
                                ParseNode fragment;
                                while (i + 1 < parent.Count - 1)
                                {
                                    ++i;
                                    fragment = SearchDownFor(parent[i], RULE_NAME_IDENTIFIER);
                                    if (fragment != null &&
                                        fragment.Value != "public" &&
                                        fragment.Value != "private" &&
                                        fragment.Value != "protected" &&
                                        fragment.Value != "virtual"
                                        )
                                        baseClasses.Add(fragment.Value);
                                }
                                meta.BaseClasses = baseClasses;
                            }
                            return meta;
                        }
                    }
                }
            }

            foreach (ParseNode child in node)
            {
                meta = SearchDownForMetaComponent(child);
                if (meta != null)
                    return meta;
            }

            return null;
        }

        private static ParseNode SearchUpForDeclarationContent(ParseNode node)
        {
            if (node == null)
                return null;

            if (node.RuleName == RULE_NAME_DECLARATION_CONTENT)
                return node;
            else
            {
                ParseNode parent = node.GetParent();
                if (parent != null)
                    return SearchUpForDeclarationContent(parent);
            }

            return null;
        }

        private static bool HasRuleWithValue(ParseNode node, string ruleName, string value, string ruleType = null)
        {
            if (node == null)
                return false;

            if (node.RuleName == ruleName && (ruleType == null || node.RuleType == ruleType) && node.Value == value)
                return true;

            foreach (ParseNode child in node)
                if (HasRuleWithValue(child, ruleName, value, ruleType))
                    return true;

            return false;
        }

        private static ParseNode SearchDownFor(ParseNode node, string ruleName, string ruleType = null)
        {
            if (node == null)
                return null;

            if (node.RuleName == ruleName && (ruleType == null || node.RuleType == ruleType))
                return node;

            ParseNode _node;
            foreach (ParseNode child in node)
            {
                _node = SearchDownFor(child, ruleName, ruleType);
                if (_node != null)
                    return _node;
            }

            return null;
        }

        private static Dictionary<string, string> SearchDownForAttributes(ParseNode node)
        {
            if (node == null)
                return null;

            Dictionary<string, string> attr;
            if (node.RuleName == RULE_NAME_PARAN_GROUP)
            {
                ParseNode declCont = SearchDownFor(node, RULE_NAME_DECLARATION_CONTENT);
                if (declCont != null && declCont.Count > 0)
                {
                    attr = new Dictionary<string, string>();
                    string attrValue = null;

                    int i = 0;
                    ParseNode identifier;
                    ParseNode value;
                    do
                    {
                        identifier = SearchDownFor(declCont[i], RULE_NAME_IDENTIFIER);
                        if (identifier != null && i + 1 < declCont.Count)
                        {
                            ++i;
                            attrValue = null;
                            value = SearchDownFor(declCont[i], RULE_NAME_PARAN_GROUP);
                            if (value != null)
                            {
                                value = SearchDownFor(value, RULE_NAME_DECLARATION_CONTENT);
                                if (value != null)
                                {
                                    value = SearchDownFor(value, RULE_NAME_STRING_LITERAL);
                                    if (value != null)
                                    {
                                        value = SearchDownFor(value, RULE_NAME_UNNAMED, RULE_TYPE_STAR);
                                        if (value != null)
                                            attrValue = value.Value;
                                    }
                                }
                            }
                            attr.Add(identifier.Value, attrValue);
                        }
                        ++i;
                    }
                    while (i < declCont.Count);

                    return attr.Count <= 0 ? null : attr;
                }
            }

            foreach (ParseNode child in node)
            {
                attr = SearchDownForAttributes(child);
                if (attr != null)
                    return attr;
            }

            return null;
        }

        private static List<MetaProperty> SearchDownForProperties(ParseNode node)
        {
            if (node == null)
                return null;

            List<MetaProperty> props;
            if (node.RuleName == RULE_NAME_BRACE_GROUP)
            {
                ParseNode decls = SearchDownFor(node, RULE_NAME_DECLARATION_LIST);
                if (decls != null)
                {
                    decls = SearchDownFor(decls, RULE_NAME_UNNAMED, RULE_TYPE_STAR);
                    if (decls != null && decls.Count > 0)
                    {
                        props = new List<MetaProperty>();

                        ParseNode propCont;
                        ParseNode declCont;
                        Dictionary<string, string> attrs;
                        MetaProperty meta;
                        foreach (ParseNode child in decls)
                        {
                            declCont = SearchDownFor(child, RULE_NAME_DECLARATION_CONTENT);
                            if (declCont != null)
                            {
                                int i = 0;
                                do
                                {
                                    if (HasRuleWithValue(declCont[i], RULE_NAME_IDENTIFIER, META_PROPERTY) && i + 3 < declCont.Count)
                                    {
                                        if (HasRuleWithValue(declCont[i + 2], RULE_NAME_IDENTIFIER, XECORE_COMMON_PROPERTY) &&
                                            HasRuleWithValue(declCont[i + 3], RULE_NAME_UNNAMED, "<", RULE_TYPE_CHARSET))
                                        {
                                            int skip = 0;
                                            int variant = -1;
                                            if (i + 8 < declCont.Count &&
                                                HasRuleWithValue(declCont[i + 5], RULE_NAME_UNNAMED, ",", RULE_TYPE_CHARSET) &&
                                                HasRuleWithValue(declCont[i + 7], RULE_NAME_UNNAMED, ">", RULE_TYPE_CHARSET))
                                            {
                                                variant = 8;
                                                skip = 8;
                                            }
                                            else if (i + 9 < declCont.Count &&
                                                (
                                                    HasRuleWithValue(declCont[i + 5], RULE_NAME_UNNAMED, "*", RULE_TYPE_CHARSET) ||
                                                    HasRuleWithValue(declCont[i + 5], RULE_NAME_UNNAMED, "&", RULE_TYPE_CHARSET)
                                                ) &&
                                                HasRuleWithValue(declCont[i + 6], RULE_NAME_UNNAMED, ",", RULE_TYPE_CHARSET) &&
                                                HasRuleWithValue(declCont[i + 8], RULE_NAME_UNNAMED, ">", RULE_TYPE_CHARSET))
                                            {
                                                variant = 9;
                                                skip = 9;
                                            }

                                            if (variant != -1)
                                            {
                                                meta = new MetaProperty();

                                                propCont = SearchDownFor(declCont[i + 4], RULE_NAME_IDENTIFIER);
                                                if (propCont != null)
                                                    meta.ValueType = propCont.Value;
                                                propCont = SearchDownFor(declCont[i + variant], RULE_NAME_IDENTIFIER);
                                                if (propCont != null)
                                                    meta.Name = propCont.Value;
                                                propCont = SearchDownFor(declCont[i + 1], RULE_NAME_PARAN_GROUP);
                                                if (propCont != null)
                                                {
                                                    attrs = SearchDownForAttributes(propCont);
                                                    if (attrs != null)
                                                    {
                                                        foreach (string attr in attrs.Keys)
                                                        {
                                                            if (attr == META_ATTR_NAME)
                                                                meta.Name = attrs[attr];
                                                            else if (attr == META_ATTR_DESCRIPTION)
                                                                meta.Description = attrs[attr];
                                                            else if (attr == META_ATTR_VALUE_TYPE)
                                                                meta.ValueType = attrs[attr];
                                                            else if (attr == META_ATTR_DEFAULT_VALUE)
                                                                meta.DefaultValue = attrs[attr].Replace("\\\"", "\"");
                                                        }
                                                    }
                                                    attrs = null;
                                                }

                                                props.Add(meta);
                                                i += skip;
                                            }
                                        }
                                    }
                                    ++i;
                                }
                                while (i < declCont.Count);
                            }
                        }

                        return props.Count > 0 ? props : null;
                    }
                }
            }

            foreach (ParseNode child in node)
            {
                props = SearchDownForProperties(child);
                if (props != null)
                    return props;
            }

            return null;
        }
    }
}
