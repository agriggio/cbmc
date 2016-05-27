typedef struct __CPROVER_string { char *s; } __CPROVER_string;
typedef struct __CPROVER_char { char c; } __CPROVER_char;

/******************************************************************************
 * CPROVER string functions
 ******************************************************************************/
/* returns s[p] */
#define __CPROVER_char_at(s, p) __CPROVER_uninterpreted_char_at(s, p)

/* string equality */
#define __CPROVER_string_equal(s1, s2) __CPROVER_uninterpreted_string_equal(s1, s2)

/* defines a string literal, e.g. __CPROVER_string_literal("foo") */
#define __CPROVER_string_literal(s) __CPROVER_uninterpreted_string_literal(s)

/* defines a char literal, e.g. __CPROVER_char_literal("c"). NOTE: you
 * *must* use a C string literal as argument (i.e. double quotes "c", not
 * single 'c') */
#define __CPROVER_char_literal(c) __CPROVER_uninterpreted_char_literal(c)

/* produces the concatenation of s1 and s2 */
#define __CPROVER_string_concat(s1, s2) __CPROVER_uninterpreted_strcat(s1, s2)

/* return the length of s */
#define __CPROVER_string_length(s) __CPROVER_uninterpreted_strlen(s)

/* extracts the substring between positions i and j */
#define __CPROVER_string_substring(s, i, j) __CPROVER_uninterpreted_substring(s, i, j)

/* test whether p is a prefix of s */
#define __CPROVER_string_isprefix(p, s) __CPROVER_uninterpreted_strprefixof(p, s)

/* test whether p is a suffix of s */
#define __CPROVER_string_issuffix(p, s) __CPROVER_uninterpreted_strsuffixof(p, s)

/* returns a new string obtained from s by setting s[p] = c */
#define __CPROVER_char_set(s, p, c) __CPROVER_uninterpreted_char_set(s, p, c)


/******************************************************************************
 * don't use these directly
 ******************************************************************************/
extern __CPROVER_char __CPROVER_uninterpreted_char_at(__CPROVER_string str, unsigned pos);
extern __CPROVER_bool __CPROVER_uninterpreted_string_equal(__CPROVER_string str1, __CPROVER_string str2);
extern __CPROVER_string __CPROVER_uninterpreted_string_literal();
extern __CPROVER_char __CPROVER_uninterpreted_char_literal();
extern __CPROVER_string __CPROVER_uninterpreted_strcat(__CPROVER_string str1, __CPROVER_string str2);
extern unsigned __CPROVER_uninterpreted_strlen(__CPROVER_string str);
extern __CPROVER_string __CPROVER_uninterpreted_substring(__CPROVER_string str, unsigned i, unsigned j);
extern __CPROVER_bool __CPROVER_uninterpreted_strprefixof(__CPROVER_string pref, __CPROVER_string str);
extern __CPROVER_bool __CPROVER_uninterpreted_strsuffixof(__CPROVER_string suff, __CPROVER_string str);
extern __CPROVER_string __CPROVER_uninterpreted_char_set(__CPROVER_string str, unsigned pos, __CPROVER_char c);
