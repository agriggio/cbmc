; SMT 2
; Generated for Z3
(set-info :source "Generated by CBMC 5.4")
(set-option :produce-models true)
; string support via PASS-style quantified arrays
(define-sort cprover.Char () (_ BitVec 8))
(define-sort cprover.Pos () (_ BitVec 32))
(define-sort cprover.String () (Array cprover.Pos cprover.Char))
(declare-fun cprover.str.len (cprover.String) cprover.Pos)

; find_symbols
(declare-fun |main::$tmp::return_value___CPROVER_uninterpreted_string_equal$5!0@1#2| () Bool)
; convert
(define-fun |B0| () Bool |main::$tmp::return_value___CPROVER_uninterpreted_string_equal$5!0@1#2|)

; convert
(define-fun |B1| () Bool |main::$tmp::return_value___CPROVER_uninterpreted_string_equal$5!0@1#2|)

; convert
(define-fun |B2| () Bool |main::$tmp::return_value___CPROVER_uninterpreted_string_equal$5!0@1#2|)

; find_symbols
(declare-fun |goto_symex::&92;guard#1| () Bool)
; convert
(define-fun |B3| () Bool (and |main::$tmp::return_value___CPROVER_uninterpreted_string_equal$5!0@1#2| |goto_symex::&92;guard#1|))

; set_to true (equal)
(define-fun |__CPROVER_dead_object#1| () (_ BitVec 32) (_ bv0 32))

; set_to true (equal)
(define-fun |__CPROVER_deallocated#1| () (_ BitVec 32) (_ bv0 32))

; set_to true (equal)
(define-fun |__CPROVER_malloc_is_new_array#1| () Bool false)

; set_to true (equal)
(define-fun |__CPROVER_malloc_object#1| () (_ BitVec 32) (_ bv0 32))

; set_to true (equal)
(define-fun |__CPROVER_malloc_size#1| () (_ BitVec 32) (_ bv0 32))

; set_to true (equal)
(define-fun |__CPROVER_memory_leak#1| () (_ BitVec 32) (_ bv0 32))

; set_to true (equal)
(define-fun |__CPROVER_next_thread_id#1| () (_ BitVec 32) (_ bv0 32))

; set_to true (equal)
(define-fun |__CPROVER_pipe_count#1| () (_ BitVec 32) (_ bv0 32))

; set_to true (equal)
(define-fun |__CPROVER_rounding_mode!0#1| () (_ BitVec 32) (_ bv0 32))

; set_to true (equal)
(define-fun |__CPROVER_thread_id!0#1| () (_ BitVec 32) (_ bv0 32))

; the following is a substitute for lambda i. x
(declare-fun array_of.0 () (Array (_ BitVec 32) Bool))
; set_to true (equal)
(define-fun |__CPROVER_threads_exited#1| () (Array (_ BitVec 32) Bool) array_of.0)

; find_symbols
(declare-fun |main::1::x!0@1#1| () cprover.String)
; find_symbols
(declare-fun |main::1::y!0@1#1| () cprover.String)
; string concatenation
(declare-fun string_concat.1 () cprover.String)
(define-fun string_concat.s0.1 () cprover.String |main::1::x!0@1#1|)
(define-fun string_concat.s1.1 () cprover.String |main::1::y!0@1#1|)
(assert (forall ((?n cprover.Pos)) (=> (bvult ?n (cprover.str.len string_concat.s0.1)) (= (select string_concat.s0.1 ?n) (select string_concat.1 ?n)))))
(assert (forall ((?n cprover.Pos)) (=> (bvult ?n (cprover.str.len string_concat.s1.1)) (= (select string_concat.s1.1 ?n) (select string_concat.1 (bvadd (cprover.str.len string_concat.s0.1) ?n))))))
(assert (= (cprover.str.len string_concat.1) (bvadd (cprover.str.len string_concat.s0.1) (cprover.str.len string_concat.s1.1))))
(assert (bvuge (cprover.str.len string_concat.1) (cprover.str.len string_concat.s0.1)))
(assert (bvuge (cprover.str.len string_concat.1) (cprover.str.len string_concat.s1.1)))

; set_to true (equal)
(define-fun |main::$tmp::return_value___CPROVER_uninterpreted_strcat$2!0@1#2| () cprover.String string_concat.1)

(declare-fun string.2 () cprover.String)
(assert (= (select string.2 (_ bv0 32)) (_ bv97 8)))
(assert (= (select string.2 (_ bv1 32)) (_ bv98 8)))
(assert (= (select string.2 (_ bv2 32)) (_ bv99 8)))
(assert (= (cprover.str.len string.2) (_ bv3 32)))
; set_to true (equal)
(define-fun |main::$tmp::return_value___CPROVER_uninterpreted_string_literal$3!0@1#2| () cprover.String string.2)

; find_symbols
(declare-fun |main::1::z!0@1#1| () cprover.String)
; string concatenation
(declare-fun string_concat.3 () cprover.String)
(define-fun string_concat.s0.3 () cprover.String |main::1::z!0@1#1|)
(define-fun string_concat.s1.3 () cprover.String |main::$tmp::return_value___CPROVER_uninterpreted_string_literal$3!0@1#2|)
(assert (forall ((?n cprover.Pos)) (=> (bvult ?n (cprover.str.len string_concat.s0.3)) (= (select string_concat.s0.3 ?n) (select string_concat.3 ?n)))))
(assert (forall ((?n cprover.Pos)) (=> (bvult ?n (cprover.str.len string_concat.s1.3)) (= (select string_concat.s1.3 ?n) (select string_concat.3 (bvadd (cprover.str.len string_concat.s0.3) ?n))))))
(assert (= (cprover.str.len string_concat.3) (bvadd (cprover.str.len string_concat.s0.3) (cprover.str.len string_concat.s1.3))))
(assert (bvuge (cprover.str.len string_concat.3) (cprover.str.len string_concat.s0.3)))
(assert (bvuge (cprover.str.len string_concat.3) (cprover.str.len string_concat.s1.3)))

; set_to true (equal)
(define-fun |main::$tmp::return_value___CPROVER_uninterpreted_strcat$4!0@1#2| () cprover.String string_concat.3)

; string equal
(declare-fun string_equal.4 () Bool)
(define-fun string_equal.s1.4 () cprover.String |main::$tmp::return_value___CPROVER_uninterpreted_strcat$2!0@1#2|)
(define-fun string_equal.s2.4 () cprover.String |main::$tmp::return_value___CPROVER_uninterpreted_strcat$4!0@1#2|)
(declare-fun string_equal.idx.4 () cprover.Pos)
(assert (=> string_equal.4 (= (cprover.str.len string_equal.s1.4) (cprover.str.len string_equal.s2.4))))
(assert (forall ((?n cprover.Pos)) (=> (and string_equal.4 (bvult ?n (cprover.str.len string_equal.s1.4))) (= (select string_equal.s1.4 ?n) (select string_equal.s2.4 ?n)))))
(assert (=> (not string_equal.4) (or (not (= (cprover.str.len string_equal.s1.4) (cprover.str.len string_equal.s2.4)))
(and (bvult string_equal.idx.4 (cprover.str.len string_equal.s1.4)) (not (= (select string_equal.s1.4 string_equal.idx.4) (select string_equal.s2.4 string_equal.idx.4)))))))

; set_to true
(assert (= |main::$tmp::return_value___CPROVER_uninterpreted_string_equal$5!0@1#2| string_equal.4))

; set_to true (equal)
(define-fun |main::$tmp::return_value___CPROVER_uninterpreted_strlen$1!0@1#2| () (_ BitVec 32) (cprover.str.len |main::1::x!0@1#1|))

; set_to true
(assert (= |goto_symex::&92;guard#1| (= |main::$tmp::return_value___CPROVER_uninterpreted_strlen$1!0@1#2| (_ bv0 32))))

; convert
(define-fun |B4| () Bool (= |main::1::x!0@1#1| |main::1::x!0@1#1|))

; convert
(define-fun |B5| () Bool (= |main::1::y!0@1#1| |main::1::y!0@1#1|))

; convert
(define-fun |B6| () Bool (= |main::1::z!0@1#1| |main::1::z!0@1#1|))

; find_symbols
(declare-fun |main::$tmp::return_value___CPROVER_uninterpreted_strcat$2!0@1#1| () cprover.String)
; convert
(define-fun |B7| () Bool (= |main::$tmp::return_value___CPROVER_uninterpreted_strcat$2!0@1#1| |main::$tmp::return_value___CPROVER_uninterpreted_strcat$2!0@1#1|))

; find_symbols
(declare-fun |main::$tmp::return_value___CPROVER_uninterpreted_string_literal$3!0@1#1| () cprover.String)
; convert
(define-fun |B8| () Bool (= |main::$tmp::return_value___CPROVER_uninterpreted_string_literal$3!0@1#1| |main::$tmp::return_value___CPROVER_uninterpreted_string_literal$3!0@1#1|))

; find_symbols
(declare-fun |main::$tmp::return_value___CPROVER_uninterpreted_strcat$4!0@1#1| () cprover.String)
; convert
(define-fun |B9| () Bool (= |main::$tmp::return_value___CPROVER_uninterpreted_strcat$4!0@1#1| |main::$tmp::return_value___CPROVER_uninterpreted_strcat$4!0@1#1|))

; find_symbols
(declare-fun |main::$tmp::return_value___CPROVER_uninterpreted_string_equal$5!0@1#1| () Bool)
; convert
(define-fun |B10| () Bool (= |main::$tmp::return_value___CPROVER_uninterpreted_string_equal$5!0@1#1| |main::$tmp::return_value___CPROVER_uninterpreted_string_equal$5!0@1#1|))

; find_symbols
(declare-fun |main::$tmp::return_value___CPROVER_uninterpreted_strlen$1!0@1#1| () (_ BitVec 32))
; convert
(define-fun |B11| () Bool (= |main::$tmp::return_value___CPROVER_uninterpreted_strlen$1!0@1#1| |main::$tmp::return_value___CPROVER_uninterpreted_strlen$1!0@1#1|))

; set_to true
(assert |main::$tmp::return_value___CPROVER_uninterpreted_string_equal$5!0@1#2|)

; set_to true
(assert |goto_symex::&92;guard#1|)

; convert
(define-fun |B12| () Bool (not |main::$tmp::return_value___CPROVER_uninterpreted_string_equal$5!0@1#2|))

; convert
(define-fun |B13| () Bool (not (= |main::$tmp::return_value___CPROVER_uninterpreted_strlen$1!0@1#2| (_ bv0 32))))

(check-sat)

(get-value (|B0|))
(get-value (|B1|))
(get-value (|B10|))
(get-value (|B11|))
(get-value (|B12|))
(get-value (|B13|))
(get-value (|B2|))
(get-value (|B3|))
(get-value (|B4|))
(get-value (|B5|))
(get-value (|B6|))
(get-value (|B7|))
(get-value (|B8|))
(get-value (|B9|))
(get-value (|__CPROVER_dead_object#1|))
(get-value (|__CPROVER_deallocated#1|))
(get-value (|__CPROVER_malloc_is_new_array#1|))
(get-value (|__CPROVER_malloc_object#1|))
(get-value (|__CPROVER_malloc_size#1|))
(get-value (|__CPROVER_memory_leak#1|))
(get-value (|__CPROVER_next_thread_id#1|))
(get-value (|__CPROVER_pipe_count#1|))
(get-value (|__CPROVER_rounding_mode!0#1|))
(get-value (|__CPROVER_thread_id!0#1|))
(get-value (|__CPROVER_threads_exited#1|))
(get-value (|goto_symex::&92;guard#1|))
(get-value (|main::$tmp::return_value___CPROVER_uninterpreted_strcat$2!0@1#1|))
(get-value (|main::$tmp::return_value___CPROVER_uninterpreted_strcat$2!0@1#2|))
(get-value (|main::$tmp::return_value___CPROVER_uninterpreted_strcat$4!0@1#1|))
(get-value (|main::$tmp::return_value___CPROVER_uninterpreted_strcat$4!0@1#2|))
(get-value (|main::$tmp::return_value___CPROVER_uninterpreted_string_equal$5!0@1#1|))
(get-value (|main::$tmp::return_value___CPROVER_uninterpreted_string_equal$5!0@1#2|))
(get-value (|main::$tmp::return_value___CPROVER_uninterpreted_string_literal$3!0@1#1|))
(get-value (|main::$tmp::return_value___CPROVER_uninterpreted_string_literal$3!0@1#2|))
(get-value (|main::$tmp::return_value___CPROVER_uninterpreted_strlen$1!0@1#1|))
(get-value (|main::$tmp::return_value___CPROVER_uninterpreted_strlen$1!0@1#2|))
(get-value (|main::1::x!0@1#1|))
(get-value (|main::1::y!0@1#1|))
(get-value (|main::1::z!0@1#1|))

(exit)
; end of SMT2 file