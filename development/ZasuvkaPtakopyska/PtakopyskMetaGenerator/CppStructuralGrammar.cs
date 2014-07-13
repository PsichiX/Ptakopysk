using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CppRipper
{
    /// <summary>
    /// This grammar is used for parsing C/C++ files into declarations, including comments.
    /// The goal is to get a high-level structure of the language, and understand 
    /// where the comments relate to items.
    /// </summary>
    class CppStructuralGrammar
        : CppBaseGrammar
    {
        public Rule paran_group;
        public Rule brace_group;
        public Rule bracketed_group;
        public Rule symbol;
        public Rule template_decl;
        public Rule typedef_decl;
        public Rule class_decl;
        public Rule struct_decl;
        public Rule enum_decl;
        public Rule union_decl;
        public Rule type_decl;
        public Rule pp_directive;
        public Rule declaration_content;
        public Rule node;
        public Rule declaration;
        public Rule declaration_list;
        public Rule same_line_comment;
        public Rule comment_set;
        public Rule label;
        public Rule file;

        public Rule Delimiter(string s)
        {
            return Skip(CharSeq(s)) + Eat(multiline_ws);
        }

        public CppStructuralGrammar()
        {
            declaration_list
                = Recursive(() => Star(declaration));

            bracketed_group
                = Delimiter("[") + declaration_list + NoFail(Delimiter("]"));

            paran_group
                = Delimiter("(") + declaration_list + NoFail(Delimiter(")"));

            brace_group
                = Delimiter("{") + declaration_list + NoFail(Delimiter("}"));

            symbol
                = Not(CharSeq("/*") | CharSeq("//")) + CharSet("~!@%^&*-+=|:<>.?/,") + Eat(multiline_ws);

            template_decl
                = TEMPLATE + NoFail(Nested("<", ">")) + ws;

            typedef_decl
                = TYPEDEF + Eat(multiline_ws);

            class_decl
                = CLASS + Opt(identifier) + Eat(multiline_ws);

            struct_decl
                = STRUCT + Opt(identifier) + Eat(multiline_ws);

            union_decl
                = UNION + Opt(identifier) + Eat(multiline_ws);

            enum_decl
                = ENUM + Opt(identifier) + Eat(multiline_ws);

            label
                = identifier + ws + COLON + Eat(multiline_ws);

            comment_set
                = Star(comment + Eat(multiline_ws)) + Eat(multiline_ws);

            same_line_comment
                = Eat(simple_ws) + comment;

            pp_directive
                = CharSeq("#") + NoFailSeq(ws + identifier + Eat(simple_ws) + until_eol + eol);

            type_decl 
                = Opt(template_decl) + (class_decl | struct_decl | union_decl | enum_decl);

            node 
                = bracketed_group
                | paran_group
                | brace_group
                | type_decl
                | typedef_decl
                | literal
                | symbol
                | label
                | identifier;

            declaration_content
                = Plus(node + Eat(multiline_ws));

            declaration
                = comment_set + pp_directive + Eat(multiline_ws)
                | comment_set + semicolon + Opt(same_line_comment) + Eat(multiline_ws)
                | comment_set + declaration_content + Opt(semicolon) + Opt(same_line_comment) + Eat(multiline_ws);

            file
                = declaration_list + ws + NoFail(EndOfInput());

            //===============================================================================================
            // Tidy up the grammar, and assign rule names from the field names.

            InitializeRules<CppStructuralGrammar>();
        }
    }
}
