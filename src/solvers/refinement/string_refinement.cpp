/** -*- C++ -*- *****************************************************\

Module: String support via axiom instantiation
        (see the PASS paper at HVC'13)

Author: Alberto Griggio, alberto.griggio@gmail.com

\*******************************************************************/

#include <solvers/refinement/string_refinement.h>
#include <ansi-c/string_constant.h>
#include <util/i2string.h>
#include <util/simplify_expr.h>
#include <solvers/sat/satcheck.h>
#include <sstream>


string_refinementt::string_refinementt(const namespacet &_ns, propt &_prop):
    SUB(_ns, _prop)
{
  string_literal_func = "__CPROVER_uninterpreted_string_literal";
  char_literal_func = "__CPROVER_uninterpreted_char_literal";
  string_length_func = "__CPROVER_uninterpreted_strlen";
  string_equal_func = "__CPROVER_uninterpreted_string_equal";
  string_char_at_func = "__CPROVER_uninterpreted_char_at";
  string_concat_func = "__CPROVER_uninterpreted_strcat";
  string_substring_func = "__CPROVER_uninterpreted_substring";
  string_is_prefix_func = "__CPROVER_uninterpreted_strprefixof";
  string_is_suffix_func = "__CPROVER_uninterpreted_strsuffixof";
  string_char_set_func = "__CPROVER_uninterpreted_char_set";
  string_length_width = 32; // TODO!
  next_symbol_id = 1;
  get_array_idxvar = fresh_symbol("get_array_idxvar", index_type());
}


string_refinementt::~string_refinementt()
{
}


void string_refinementt::post_process()
{
  for (size_t i = 0; i < cur.size(); ++i) {
    set_to_true(cur[i]);
  }
  
  // Ackermann expansion for string lengths
  for (expr_mapt::iterator i = string2length.begin(), end = string2length.end();
       i != end; ++i) {
    exprt si = make_array(i->first);
    exprt leni = i->second;
    
    expr_mapt::iterator j = i;
    for (++j; j != end; ++j) {
      exprt sj = make_array(j->first);
      exprt lenj = j->second;

      implies_exprt lemma(equal_exprt(si, sj), equal_exprt(leni, lenj));
      //prop.l_set_to_true(convert(lemma));
      set_to_true(lemma);
    }
  }

  add_instantiations(true);

  SUB::post_process();
}


bool string_refinementt::boolbv_set_equality_to_true(const equal_exprt &expr)
{
  if (!is_string_type(expr.lhs().type())) {
    return SUB::boolbv_set_equality_to_true(expr);
  }
  convert(expr);
  exprt a = make_array(expr.lhs());
  exprt b = make_array(expr.rhs());
  equal_exprt e(a, b);
  prop.l_set_to_true(record_array_equality(e));
  return false;
}


literalt string_refinementt::convert_rest(const exprt &expr)
{
  if (expr.id() == ID_function_application) {
    function_application_exprt f = to_function_application_expr(expr);
    const exprt &name = f.function();

    // check if this is something we recognize
    if (name.id() == ID_symbol) {
      const irep_idt &id = to_symbol_expr(name).get_identifier();
      if (id == string_equal_func) {
        return convert_string_equal(f)[0];
      } else if (id == string_is_prefix_func) {
        return convert_string_is_prefix(f)[0];
      } else if (id == string_is_suffix_func) {
        return convert_string_is_suffix(f)[0];
      }
    }
  }
  
  return SUB::convert_rest(expr);
}


bvt string_refinementt::convert_symbol(const exprt &expr)
{
  const typet &type = expr.type();
  const irep_idt &identifier = expr.get(ID_identifier);
  
  if (is_string_type(type)) {
    bvt ret = convert_bv(make_array(expr));
    make_length(expr); // ensure there is a length for this string
    map.set_literals(identifier, type, ret);
    return ret;
  } else if (is_char_type(expr.type())) {
    symbol_exprt c = fresh_symbol("char", char_type());
    bvt ret = SUB::convert_symbol(c);
    map.set_literals(identifier, type, ret);
    return ret;
  } else {
    return SUB::convert_symbol(expr);
  }
}


bvt string_refinementt::convert_struct(const struct_exprt &expr)
{
  if (is_string_type(expr.type()) || is_char_type(expr.type())) {
    return bvt();
  } else {
    return SUB::convert_struct(expr);
  }
}


bvt string_refinementt::convert_function_application(
  const function_application_exprt &expr)
{
  const exprt &name = expr.function();

  // check if this is something we recognize
  if (name.id() == ID_symbol) {
    const irep_idt &id = to_symbol_expr(name).get_identifier();
    if (id == string_literal_func) {
      return convert_string_literal(expr);
    } else if (id == char_literal_func) {
      return convert_char_literal(expr);
    } else if (id == string_length_func) {
      return convert_string_length(expr);
    } else if (id == string_equal_func) {
      return convert_string_equal(expr);
    } else if (id == string_char_at_func) {
      return convert_string_char_at(expr);
    } else if (id == string_concat_func) {
      return convert_string_concat(expr);
    } else if (id == string_substring_func) {
      return convert_string_substring(expr);
    } else if (id == string_is_prefix_func) {
      return convert_string_is_prefix(expr);
    } else if (id == string_is_suffix_func) {
      return convert_string_is_suffix(expr);
    } else if (id == string_char_set_func) {
      return convert_string_char_set(expr);
    }
  }

  return SUB::convert_function_application(expr);
}


void string_refinementt::check_SAT()
{
  SUB::check_SAT();
  if (!progress) {
    if (!check_axioms()) {
      progress = true;
      add_instantiations();
    }
  }
}


bool string_refinementt::is_string_type(const typet &type)
{
  if (type.id() == ID_struct) {
    irep_idt tag = to_struct_type(type).get_tag();
    return tag == irep_idt("__CPROVER_string");
  }
  return false;
}


bool string_refinementt::is_char_type(const typet &type)
{
  if (type.id() == ID_struct) {
    irep_idt tag = to_struct_type(type).get_tag();
    return tag == irep_idt("__CPROVER_char");
  }
  return false;
}


bvt string_refinementt::convert_bool_bv(const exprt &boole, const exprt &orig)
{
  bvt ret;
  ret.push_back(convert(boole));
  size_t width = boolbv_width(orig.type());
  for (size_t i = 1; i < width; ++i) {
    ret.push_back(const_literal(false));
  }
  return ret;
}


bvt string_refinementt::convert_string_equal(
  const function_application_exprt &f)
{
  symbol_exprt eq = fresh_symbol("equal");
  bvt bv = convert_bool_bv(eq, f);   

  const function_application_exprt::argumentst &args = f.arguments();
  expect(args.size() == 2, "bad args to string equal");

  const exprt &s1 = args[0];
  const exprt &s2 = args[1];

  symbol_exprt witness = fresh_symbol("index", index_type());
  exprt s1len = make_length(s1);
  exprt s2len = make_length(s2);

  implies_exprt lemma1(eq, equal_exprt(s1len, s2len));
  add_lemma(lemma1);

  string_axiomt a(string_axioms.size());
  a.idx = witness;
  a.lit = eq;
  exprt s1arr = make_array(s1);
  exprt s2arr = make_array(s2);
  a.premise = and_exprt(eq, binary_relation_exprt(witness, ID_lt, s1len));
  a.body = equal_exprt(index_exprt(s1arr, witness),
                       index_exprt(s2arr, witness));
  string_axioms.push_back(a);

  implies_exprt lemma2(
    not_exprt(eq),
    or_exprt(notequal_exprt(s1len, s2len),
             and_exprt(binary_relation_exprt(witness, ID_lt, s1len),
                       notequal_exprt(index_exprt(s1arr, witness),
                                      index_exprt(s2arr, witness)))));
  add_lemma(lemma2);

  return bv;
}


bvt string_refinementt::convert_string_length(
  const function_application_exprt &f)
{
  bvt bv;
  const function_application_exprt::argumentst &args = f.arguments();
  expect(args.size() == 1, "bad args to string length");

  exprt len = make_length(args[0]);
  bv = convert_bv(len);
  return bv;
}


bvt string_refinementt::convert_string_concat(
  const function_application_exprt &f)
{
  bvt bv;
  const function_application_exprt::argumentst &args = f.arguments();
  expect(args.size() == 2, "bad args to string concat");
  
  exprt arr = make_array(f);
  bv = convert_bv(arr);
  
  const exprt &s1 = args[0];
  const exprt &s2 = args[1];

  exprt len = make_length(f);
  exprt s1len = make_length(s1);
  exprt s2len = make_length(s2);

  exprt s1arr = make_array(s1);
  exprt s2arr = make_array(s2);

  equal_exprt lemma1(len, plus_exprt(s1len, s2len));
  add_lemma(lemma1);

  binary_relation_exprt lemma2(len, ID_ge, s1len);
  add_lemma(lemma2);

  binary_relation_exprt lemma3(len, ID_ge, s2len);
  add_lemma(lemma3);

  symbol_exprt idx = fresh_symbol("index", index_type());

  string_axiomt a1(string_axioms.size());
  a1.idx = idx;
  a1.lit = nil_exprt();
  a1.premise = binary_relation_exprt(idx, ID_lt, s1len);
  a1.body = equal_exprt(index_exprt(s1arr, idx),
                        index_exprt(arr, idx));
  string_axioms.push_back(a1);

  string_axiomt a2(string_axioms.size());
  a2.idx = idx;
  a2.lit = nil_exprt();
  a2.premise = binary_relation_exprt(idx, ID_lt, s2len);
  a2.body = equal_exprt(index_exprt(s2arr, idx),
                        index_exprt(arr, plus_exprt(s1len, idx)));
  string_axioms.push_back(a2);

  return bv;
}


bvt string_refinementt::convert_string_substring(
  const function_application_exprt &f)
{
  bvt bv;
  const function_application_exprt::argumentst &args = f.arguments();  
  expect(args.size() == 3, "bad args to string substring");

  exprt arr = make_array(f);
  bv = convert_bv(arr);

  exprt len = make_length(f);

  exprt sarr = make_array(args[0]);
  exprt slen = make_length(args[0]);
  typecast_exprt i(args[1], index_type());
  typecast_exprt j(args[2], index_type());

  exprt idx = fresh_symbol("index", index_type());

  string_axiomt a(string_axioms.size());
  a.idx = idx;
  a.lit = nil_exprt();
  a.premise = binary_relation_exprt(idx, ID_lt, len);
  a.body = equal_exprt(index_exprt(arr, idx),
                       index_exprt(sarr, plus_exprt(i, idx)));
  string_axioms.push_back(a);

  and_exprt lemma1(binary_relation_exprt(i, ID_lt, j),
                   and_exprt(binary_relation_exprt(j, ID_le, slen),
                             equal_exprt(len, minus_exprt(j, i))));
  add_lemma(lemma1);

  binary_relation_exprt lemma2(slen, ID_ge, len);
  add_lemma(lemma2);

  return bv;
}


bvt string_refinementt::convert_string_is_prefix(
  const function_application_exprt &f)
{
  bvt bv;
  const function_application_exprt::argumentst &args = f.arguments();
  expect(args.size() == 2, "bad args to string isprefix");

  symbol_exprt isprefix = fresh_symbol("isprefix");
  bv = convert_bool_bv(isprefix, f);

  exprt slen = make_length(args[0]);
  exprt sarr = make_array(args[0]);
  exprt s1len = make_length(args[1]);
  exprt s1arr = make_array(args[1]);

  implies_exprt lemma1(isprefix, binary_relation_exprt(slen, ID_ge, s1len));
  add_lemma(lemma1);

  symbol_exprt witness = fresh_symbol("index", index_type());

  string_axiomt a(string_axioms.size());
  a.idx = witness;
  a.lit = isprefix;
  a.premise = and_exprt(isprefix, binary_relation_exprt(witness, ID_lt, s1len));
  a.body = equal_exprt(index_exprt(s1arr, witness),
                       index_exprt(sarr, witness));
  string_axioms.push_back(a);

  implies_exprt lemma2(
    not_exprt(isprefix),
    or_exprt(not_exprt(binary_relation_exprt(slen, ID_ge, s1len)),
             and_exprt(binary_relation_exprt(witness, ID_lt, s1len),
                       notequal_exprt(index_exprt(s1arr, witness),
                                      index_exprt(sarr, witness)))));
  add_lemma(lemma2);

  return bv;
}


bvt string_refinementt::convert_string_is_suffix(
  const function_application_exprt &f)
{
  bvt bv;
  const function_application_exprt::argumentst &args = f.arguments();
  expect(args.size() == 2, "bad args to string issuffix");

  symbol_exprt issuffix = fresh_symbol("issuffix");
  bv = convert_bool_bv(issuffix, f);

  exprt slen = make_length(args[0]);
  exprt sarr = make_array(args[0]);
  exprt s1len = make_length(args[1]);
  exprt s1arr = make_array(args[1]);

  implies_exprt lemma1(issuffix, binary_relation_exprt(slen, ID_ge, s1len));
  add_lemma(lemma1);

  symbol_exprt witness = fresh_symbol("index", index_type());

  string_axiomt a(string_axioms.size());
  a.idx = witness;
  a.lit = issuffix;
  a.premise = and_exprt(issuffix, binary_relation_exprt(witness, ID_lt, s1len));
  a.body = equal_exprt(
    index_exprt(s1arr, witness),
    index_exprt(sarr,
                plus_exprt(witness, minus_exprt(slen, s1len))));
  string_axioms.push_back(a);

  implies_exprt lemma2(
    not_exprt(issuffix),
    or_exprt(not_exprt(binary_relation_exprt(slen, ID_ge, s1len)),
             and_exprt(binary_relation_exprt(witness, ID_lt, s1len),
                       notequal_exprt(
                         index_exprt(s1arr, witness),
                         index_exprt(sarr,
                                     plus_exprt(witness,
                                                minus_exprt(slen, s1len)))))));
  add_lemma(lemma2);

  return bv;
}


bvt string_refinementt::convert_string_literal(
  const function_application_exprt &f)
{
  bvt bv;
  const function_application_exprt::argumentst &args = f.arguments();
  expect(args.size() == 1, "bad args to string literal");

  const exprt &arg = args[0];
  if (arg.operands().size() == 1 &&
      arg.operands()[0].operands().size() == 1 &&
      arg.operands()[0].operands()[0].operands().size() == 2 &&
      arg.operands()[0].operands()[0].operands()[0].id() == ID_string_constant){
    const exprt &s = arg.operands()[0].operands()[0].operands()[0];
    irep_idt sval = to_string_constant(s).get_value();
    exprt arr = make_array(f);
    bv = convert_bv(arr);

    for (std::size_t i = 0; i < sval.size(); ++i) {
      constant_exprt idx(i2idx(i), index_type());
      constant_exprt c(i2chr(int(sval[i])), char_type());
      equal_exprt lemma(index_exprt(arr, idx), c);
      add_lemma(lemma);
    }
    exprt len = make_length(f);
    equal_exprt lemma(len, constant_exprt(i2idx(sval.size()), index_type()));
    add_lemma(lemma);
  } else {
    expect(false, "bad arg to string literal");
  }

  return bv;
}


bvt string_refinementt::convert_char_literal(
  const function_application_exprt &f)
{
  bvt bv;
  const function_application_exprt::argumentst &args = f.arguments();
  expect(args.size() == 1, "bad args to char literal");
  
  const exprt &arg = args[0];
  if (arg.operands().size() == 1 &&
      arg.operands()[0].operands().size() == 1 &&
      arg.operands()[0].operands()[0].operands().size() == 2 &&
      arg.operands()[0].operands()[0].operands()[0].id() == ID_string_constant){
    const exprt &s = arg.operands()[0].operands()[0].operands()[0];
    irep_idt sval = to_string_constant(s).get_value();
    expect(sval.size() == 1, "bad literal in char literal");

    bv = convert_bv(constant_exprt(i2chr(int(sval[0])), char_type()));
  } else {
    expect(false, "char literal");
  }

  return bv;
}


bvt string_refinementt::convert_string_char_at(
  const function_application_exprt &f)
{
  bvt bv;
  const function_application_exprt::argumentst &args = f.arguments();
  expect(args.size() == 2, "bad args to string_char_at");

  exprt arr = make_array(args[0]);
  typecast_exprt pos(args[1], index_type());
  bv = convert_bv(index_exprt(arr, pos));
  return bv;
}


bvt string_refinementt::convert_string_char_set(
  const function_application_exprt &f)
{
  bvt bv;
  const function_application_exprt::argumentst &args = f.arguments();  
  expect(args.size() == 3, "bad args to string_char_set");

  exprt arr = make_array(f);
  bv = convert_bv(arr);
  exprt len = make_length(f);
  
  exprt sarr = make_array(args[0]);
  exprt slen = make_length(args[0]);
  typecast_exprt idx(args[1], index_type());

  symbol_exprt c = fresh_symbol("char", char_type());
  bvt bva = convert_bv(args[2]);
  bvt bvc = convert_bv(c);
  bva.resize(bvc.size(), const_literal(false));
  for (size_t i = 0; i < bvc.size(); ++i) {
    prop.set_equal(bva[i], bvc[i]);
  }

  with_exprt sarrnew(sarr, idx, c);
  implies_exprt lemma(binary_relation_exprt(idx, ID_lt, slen),
                      and_exprt(equal_exprt(arr, sarrnew),
                                equal_exprt(len, slen)));
  add_lemma(lemma);

  return bv;
}


void string_refinementt::add_lemma(const exprt &lemma, bool immediately)
{
  cur.push_back(lemma);
  if (immediately) {
    //prop.l_set_to_true(convert(lemma));
    set_to_true(lemma);
  }
}


void string_refinementt::add_instantiations(bool first)
{
  if (first) {
    for (size_t i = 0; i < string_axioms.size(); ++i) {
      update_index_set(string_axioms[i]);
    }
  }
  for (size_t i = 0; i < cur.size(); ++i) {
    update_index_set(cur[i]);
  }

  cur.clear();
  size_t added = 0;

  for (index_sett::iterator i = index_set.begin(), end = index_set.end();
       i != end; ++i) {
    const exprt &s = i->first;
    for (expr_sett::const_iterator j = i->second.begin(), end = i->second.end();
         j != end; ++j) {
      const exprt &val = *j;
      for (size_t k = 0; k < string_axioms.size(); ++k) {
        exprt lemma = instantiate(string_axioms[k], s, val);
        if (lemma.is_not_nil() && seen_instances.insert(lemma).second) {
          add_lemma(lemma, true);
          ++added;
        }
      }
    }
  }

  status() << "string-refinement: added " << added << " axiom instances"
           << eom;
}


bool string_refinementt::check_axioms()
{
  // build the interpretation from the model of the prop_solver
  replace_mapt fmodel;
  for (expr_mapt::iterator it = string2length.begin(),
         end = string2length.end(); it != end; ++it) {
    const exprt &s = it->first;
    const exprt &l = it->second;
    expr_mapt::iterator j = string2array.find(s);
    assert(j != string2array.end());
    const exprt &a = j->second;

    exprt len = get(l);
    exprt arr = get_array(a, len);
    fmodel[l] = len;
    fmodel[a] = arr;
  }

  std::vector< std::pair<size_t, exprt> > violated;

  for (size_t i = 0; i < string_axioms.size(); ++i) {
    const string_axiomt &axiom = string_axioms[i];
    if (axiom.lit.is_not_nil()) {
      exprt lit = get(axiom.lit);
      fmodel[axiom.lit] = lit;
    }

    exprt negaxiom = and_exprt(axiom.premise, not_exprt(axiom.body));
    eval_axiom_in_model(fmodel, negaxiom);
    // replace_expr(fmodel, negaxiom);
//    negaxiom = simplify_expr(negaxiom, ns);

    satcheck_no_simplifiert sat_check;
    bv_pointerst solver(ns, sat_check);
    solver << negaxiom;

    switch (solver()) {
    case decision_proceduret::D_SATISFIABLE: {
      exprt val = solver.get(axiom.idx);
      violated.push_back(std::make_pair(i, val));
    } break;
    case decision_proceduret::D_UNSATISFIABLE:
      break;
    default:
      expect(false, "failure in checking axiom");
    }
  }

  if (violated.empty()) {
    status() << "string-refinement: no axiom violation found" << eom;
    return true;
  }

  for (size_t i = 0; i < violated.size(); ++i) {
    const exprt &val = violated[i].second;
    status() << "string-refinement: axiom " << violated[i].first
             << " violated by index "
             << to_constant_expr(val).get_value() << eom;
    const string_axiomt &axiom = string_axioms[violated[i].first];
    exprt premise(axiom.premise);
    exprt body(axiom.body);
    replace_expr(axiom.idx, val, premise);
    replace_expr(axiom.idx, val, body);
    implies_exprt instance(premise, body);
    if (seen_instances.insert(instance).second) {
      add_lemma(instance, true);
    }
    // TODO - add backwards instantiations
  }

  status() << "string-refinement: found " << violated.size()
           << " violated axioms" << eom;
  
  return false;
}


void string_refinementt::eval_axiom_in_model(const replace_mapt &model,
                                             exprt &axiom)
{
  replace_mapt::const_iterator it = model.find(axiom);

  if (it != model.end()) {
      axiom = it->second;
      return;
  }

  if (axiom.id() == ID_index) {
    index_exprt &i = to_index_expr(axiom);
    it = model.find(i.array());
    assert(it != model.end());
    exprt ite = it->second;
    replace_expr(get_array_idxvar, i.index(), ite);
    axiom = ite;
  }

  Forall_operands(it, axiom) {
    eval_axiom_in_model(model, *it);
  }
}


namespace {

template <class T> std::string i2bin(T n, size_t w)
{
  size_t r(n);
  std::string ret(w, '0');
  for (size_t i = 0; i < w; ++i) {
    if (r & (size_t(1) << i)) {
      ret[(w-1) - i] = '1';
    }
  }
  return ret;
}

void get_bounds(const exprt &qvar, const exprt &expr, std::vector<exprt> &out)
{
  if (expr.id() == ID_lt && expr.op0() == qvar) {
    const exprt &b = expr.op1();
    constant_exprt one(i2bin(1, to_bitvector_type(b.type()).get_width()),
                       b.type());
    out.push_back(minus_exprt(b, one));
  } else if (expr.id() == ID_le && expr.op0() == qvar) {
    out.push_back(expr.op1());
  } else {
    forall_operands(it, expr) {
      get_bounds(qvar, *it, out);
    }
  }
}


struct stop_visit {};

class find_index_visitor: public const_expr_visitort {
public:
  find_index_visitor(const exprt &str):
    str_(str)
  {
    idx = nil_exprt();
  }
  
  void operator()(const exprt &expr)
  {
    if (expr.id() == ID_index) {
      const index_exprt &i = to_index_expr(expr);
      if (i.array() == str_) {
        idx = i.index();
        throw stop_visit();
      }
    }
  }

  const exprt &str_;
  exprt idx;
};


class find_qvar_visitor: public const_expr_visitort {
public:
  find_qvar_visitor(const exprt &qvar):
    qvar_(qvar), found(false) {}

  void operator()(const exprt &expr)
  {
    if (expr == qvar_) {
      found = true;
      throw stop_visit();
    }
  }

  const exprt &qvar_;
  bool found;
};


exprt compute_subst(const exprt &qvar, const exprt &val, const exprt &f)
{
  std::vector< std::pair<exprt, bool> > to_process, elems;
  to_process.push_back(std::make_pair(f, true));

  while (!to_process.empty()) {
    exprt cur = to_process.back().first;
    bool positive = to_process.back().second;
    to_process.pop_back();

    if (cur.id() == ID_plus) {
      to_process.push_back(std::make_pair(cur.op1(), positive));
      to_process.push_back(std::make_pair(cur.op0(), positive));
    } else if (cur.id() == ID_minus) {
      to_process.push_back(std::make_pair(cur.op1(), !positive));
      to_process.push_back(std::make_pair(cur.op0(), positive));
    } else if (cur.id() == ID_unary_minus) {
      to_process.push_back(std::make_pair(cur.op0(), !positive));
    } else {
      elems.push_back(std::make_pair(cur, positive));
    }
  }

  exprt ret = nil_exprt();
  bool neg = false;
  bool found = false;
  
  for (size_t i = 0; i < elems.size(); ++i) {
    exprt &t = elems[i].first;
    if (t == qvar) {
      assert(!found);
      found = true;
      neg = !elems[i].second;
    } else {
      if (!elems[i].second) {
        t = unary_minus_exprt(t);
      }
      if (ret.is_nil()) {
        ret = t;
      } else {
        ret = plus_exprt(ret, t);
      }
    }
  }

  assert(found);
  if (!ret.is_nil()) {
    ret = minus_exprt(val, ret);
  } else {
    ret = val;
  }

  if (neg) {
    ret = unary_minus_exprt(ret);
  }

  return ret;
}
  
} // namespace


void string_refinementt::update_index_set(const string_axiomt &axiom)
{
  std::vector<exprt> bounds;
  get_bounds(axiom.idx, axiom.premise, bounds);

  std::vector<exprt> to_process;
  to_process.push_back(axiom.body);

  while (!to_process.empty()) {
    exprt cur = to_process.back();
    to_process.pop_back();
    if (cur.id() == ID_index) {
      const exprt &s = cur.op0();
      const exprt &i = cur.op1();

      find_qvar_visitor v(axiom.idx);
      expr_sett &idxs = index_set[s];
      idxs.insert(bounds.begin(), bounds.end());
      try {
        i.visit(v);
      } catch (stop_visit &) {}
      if (!v.found) {
        idxs.insert(i);
      }
    } else {
      forall_operands(it, cur) {
        to_process.push_back(*it);
      }
    }
  }
}


void string_refinementt::update_index_set(const exprt &formula)
{
  std::vector<exprt> to_process;
  to_process.push_back(formula);

  while (!to_process.empty()) {
    exprt cur = to_process.back();
    to_process.pop_back();
    if (cur.id() == ID_index) {
      const exprt &s = cur.op0();
      const exprt &i = cur.op1();

      index_set[s].insert(i);
    } else {
      forall_operands(it, cur) {
        to_process.push_back(*it);
      }
    }
  }
}


exprt string_refinementt::instantiate(const string_axiomt &axiom,
                                      const exprt &str, const exprt &val)
{
  find_index_visitor v1(str);
  try {
    axiom.body.visit(v1);
  } catch (stop_visit &) {}

  if (v1.idx.is_nil()) {
    return nil_exprt();
  }

  find_qvar_visitor v2(axiom.idx);
  try {
    v1.idx.visit(v2);
  } catch (stop_visit &) {}

  if (!v2.found) {
    return nil_exprt();
  }

  exprt r = compute_subst(axiom.idx, val, v1.idx);
  exprt premise(axiom.premise);
  replace_expr(axiom.idx, r, premise);
  exprt body(axiom.body);
  replace_expr(axiom.idx, r, body);
  implies_exprt instance(premise, body);

  return instance;
}


symbol_exprt string_refinementt::fresh_symbol(const irep_idt &prefix,
                                              const typet &tp)
{
  std::ostringstream buf;
  buf << "string_refinement#" << prefix << "#" << (next_symbol_id++);
  std::string s = buf.str();
  irep_idt name(s.c_str());
  return symbol_exprt(name, tp);
}


std::string string_refinementt::i2idx(size_t n)
{
  return i2bin(n, string_length_width);
}


std::string string_refinementt::i2chr(int n)
{
  return i2bin(n, 8);
}


typet string_refinementt::index_type()
{
  return unsignedbv_typet(string_length_width);
}


typet string_refinementt::char_type()
{
  return unsignedbv_typet(8);
}


exprt string_refinementt::make_length(const exprt &str)
{
  expr_mapt::iterator it = string2length.find(str);
  if (it != string2length.end()) {
    return it->second;
  }
  symbol_exprt len = fresh_symbol("string_length", index_type());
  string2length[str] = len;
  length2string[len] = str;
  return len;
}


exprt string_refinementt::make_array(const exprt &str)
{
  expr_mapt::iterator it = string2array.find(str);
  if (it != string2array.end()) {
    return it->second;
  }
  symbol_exprt arr = fresh_symbol("string_array",
                                  array_typet(char_type(),
                                              infinity_exprt(integer_typet())));
  string2array[str] = arr;
  return arr;
}


exprt string_refinementt::get_array(const exprt &arr, const exprt &size)
{
  exprt val = get(arr);
  expect(val.id() == "array-list", "unable to get array-list value");

  exprt ret = to_unsignedbv_type(char_type()).zero_expr();
    // array_of_exprt(to_unsignedbv_type(char_type()).zero_expr(),
    //                // array_typet(char_type(), size));
    //                array_typet(char_type(), infinity_exprt(integer_typet())));

  for (size_t i = 0; i < val.operands().size()/2; ++i) {
    exprt tmp_index = val.operands()[i*2];
    typecast_exprt idx(tmp_index, index_type());
    exprt tmp_value = val.operands()[i*2+1];
    typecast_exprt value(tmp_value, char_type());
    ret = if_exprt(equal_exprt(get_array_idxvar, idx), value, ret);
  }

  return ret;
}


void string_refinementt::expect(bool cond, const char *msg)
{
  assert(cond);
  if (!cond) {
    throw (msg ? msg : "assertion failure!");
  }
}
