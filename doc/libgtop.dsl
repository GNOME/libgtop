<!doctype style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN">
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;
; dbtohtml.dsl - DSSSL style sheet for DocBook to HTML conversion (jadeware)
;
; Author          : Mark Burton (markb@ordern.com)
; Created On      : Fri Jun 13 18:21:14 1997
; Last Modified By: Mark Burton
; Last Modified On: Thu Jul 10 21:58:53 1997
;
; $Id$
;
; Usage:
;
; jade -d dbtohtml.dsl -t sgml yourdoc.sgm
;
; Additional command line options:
;
; -V %no-split-output%  sends all the output to one file
; -V %no-make-index%    disables index creation
; -V %no-make-toc%      disables TOC creation
; -V %no-shade-screen%  disables grey background to SCREEN regions
; -V %show-comments%    includes contents of COMMENT regions

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Contributors

; Mark Eichin (eichin@cygnus.com)
; Jason Molenda (crash@cygnus.co.jp)
; Mark Galassi (rosalia@cygnus.com)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Parameterisation

; This style sheet can easily be parameterised by the use of a driver.
; Here is a simple example that sets the output file basename and directory.
; If the driver is foo.dsl, use: jade -d foo.dsl -t sgml yourdoc.sgm

<!--

<!doctype style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN" [
<!ENTITY dbtohtml.dsl SYSTEM "dbtohtml.dsl" CDATA DSSSL >
]>

<style-specification id="foodbtohtml" use="dbtohtml">

(define %output-basename% "foo")
(define %output-directory% "foodir")

</style-specification>

<external-specification id="dbtohtml" document="dbtohtml.dsl">

-->

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; declare non-standard functions

(declare-flow-object-class element
  "UNREGISTERED::James Clark//Flow Object Class::element")
(declare-flow-object-class empty-element
  "UNREGISTERED::James Clark//Flow Object Class::empty-element")
(declare-flow-object-class document-type
  "UNREGISTERED::James Clark//Flow Object Class::document-type")
(declare-flow-object-class processing-instruction
  "UNREGISTERED::James Clark//Flow Object Class::processing-instruction")
(declare-flow-object-class entity
  "UNREGISTERED::James Clark//Flow Object Class::entity")
(declare-flow-object-class entity-ref
  "UNREGISTERED::James Clark//Flow Object Class::entity-ref")
(declare-flow-object-class formatting-instruction
  "UNREGISTERED::James Clark//Flow Object Class::formatting-instruction")

(declare-characteristic preserve-sdata?
  "UNREGISTERED::James Clark//Characteristic::preserve-sdata?" #f)
;;
;; try setting this to true to see if ISO character entities get translated
;;(declare-characteristic preserve-sdata?
;;  "UNREGISTERED::James Clark//Characteristic::preserve-sdata?" #t)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; variables

(define %no-split-output% #f)
(define %no-make-toc% #f)
(define %no-make-index% #f)
(define %no-shade-screen% #f)
(define %show-comments% #f)
(define %shade-width% "100%")		; width or #f
(define %email-element% "TT")		; font changing element or #f

(define %html-public-id% "-//W3C//DTD HTML 3.2 Final//EN")
(define %body-bgcolor% "white")
(define %output-directory% ".")
(define %output-basename% "libgtop")
(define %output-suffix% ".shtml")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; top-level sections

(define (book-common)
  (cond (%no-split-output%		; everything goes in one file
	 (make-file (string-append %output-basename% %output-suffix%)
		    (make sequence
		      (process-children)
		      (cond ((not %no-make-index%)
			     (make sequence
			       (make-fat-rule)
			       (make-index)))
			    (#t (empty-sosofo))))))
	(#t				; split output into separate files
	 (make sequence
	   (make-file (string-append %output-basename% %output-suffix%)
		      (make sequence
			(process-first-descendant "TITLE")
			(process-first-descendant "BOOKINFO")))
	   (process-matching-children "PREFACE"
				      "CHAPTER"
				      "APPENDIX"
				      "BIBLIOGRAPHY"
				      "GLOSSARY"
				      "ARTHEADER")
	   (cond ((not %no-make-index%)
		  (make-file (string-append %output-basename%
					    "-INDEX"
					    %output-suffix%)
			     (make-index)))
		 (#t (empty-sosofo)))))))

(element BOOK (book-common))

(define generator-version
	"Generated from Docbook 3.0 DTD, dbtohtml.dsl version $Revision$")

(define (make-file file-name content)
  (make entity
    system-id: (string-append %output-directory% "/" file-name)
    (make sequence
      (make document-type
	name: "html"
	public-id: %html-public-id%)
      (make formatting-instruction data: "<")
      (make formatting-instruction data: "!--")
      (literal generator-version)
      (make formatting-instruction data: "-->
")
      (make element
	gi: "html"
	(make sequence
	  (make element
	    gi: "head"
	    (make sequence
	      (make empty-element gi: "link"
		attributes: (list (list "rel" "stylesheet")
				  (list "type" "text/css")
				  (list "href" "/style/docbook.css")))
	      (make empty-element gi: "meta"
		    attributes: (list (list "name" "generator")
				      (list "content" generator-version)))
	      (make empty-element gi: "meta"
		    attributes: (list (list "name" "ObjectType")
				      (list "content" "book")))
	      (make element
		gi: "title"
		(with-mode extract-title-text
		  (process-first-descendant "title")))))
	  (make element
	    gi: "body"
	    attributes: (list (list "bgcolor" %body-bgcolor%))
	    (make sequence
	      (make-anchor)
	      content
	      (make-footer)
              (make formatting-instruction data: "<")
              (make formatting-instruction data: "!--#include virtual=\"/includes/docbook_footer\"")
              (make formatting-instruction data: "-->"))))))))

(define (make-footer)
  (let ((copyright (select-elements (descendants (book-node))
				    '("COPYRIGHT" "BOOKINFO"))))
    (cond ((node-list-empty? copyright) (empty-sosofo))
	  (#t (make sequence
		(make-fat-rule)
		(process-node-list copyright))))))

(define (node-list-last nl)
  (node-list-ref nl (- (node-list-length nl) 1)))

(define (make-nav-links parent-gi)
  (make sequence
    (make empty-element
      gi: "P")
    (make element
      gi: "A"
      attributes: (list (list "href" (link-file-name (ancestor parent-gi))))
      (literal "Up"))
    (literal " ")
    (if (absolute-last-sibling?)
	(empty-sosofo)
	(make element
	  gi: "A"
	  attributes: (list (list "href"
				  (link-file-name (node-list-first
						   (follow (current-node))))))
	  (literal "Forward")))
    (literal " ")
    (if (absolute-first-sibling?)
	(empty-sosofo)
	(make element
	  gi: "A"
	  attributes: (list (list "href"
				  (link-file-name (node-list-last
						   (preced (current-node))))))
	  (literal "Back")))
    (make empty-element
      gi: "P")))

(define (make-pref-chap-app)
  (cond (%no-split-output%
	 (make sequence
	   (make-anchor)
	   (make-fat-rule)
	   (process-children)))
	(#t
	 (make-file (link-file-name (current-node))
		    (make sequence
		      (make-nav-links "BOOK")
		      (process-children)
		      (make-nav-links "BOOK"))))))

;;(element ARTICLE (process-children))
(element ARTICLE (book-common))

(element PREFACE (make-pref-chap-app))

(element CHAPTER (make-pref-chap-app))

(element APPENDIX (make-pref-chap-app))

(element BEGINPAGE (make sequence
			(make empty-element gi: "P")
			(make empty-element gi: "HR")))

(element BIBLIOGRAPHY (make-pref-chap-app))

(element BOOKBIBLIO (process-children))

(element BIBLIODIV (process-children))

(element GLOSSARY (make-pref-chap-app))

; (element GLOSSDIV (make-pref-chap-app))

;;(element ARTHEADER (make-pref-chap-app))
(element ARTHEADER
  (make sequence
    (cond ((not %no-make-toc%)
	   (make sequence
	     (make-fat-rule)
	     (make element
	       gi: "h2"
	       attributes: (list "align" "center")
	       (literal "Contents"))
	     (make element
	       gi: "ul"
	       attributes: (list "align" "center")
	       (with-mode make-toc-links
		 (process-node-list (book-node))))))
	  (#t (empty-sosofo)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; sections

(element SECT1
  (make sequence
    (make-anchor)
    (process-children)))

(element SECT2
  (make sequence
    (make-anchor)
    (process-children)))

(element SECT3
  (make sequence
    (make-anchor)
    (process-children)))

(element SECT4
  (make sequence
    (make-anchor)
    (process-children)))

(element SECT5
  (make sequence
    (make-anchor)
    (process-children)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; titles

(mode extract-title-text
  (element (TITLE)
    (process-children)))

(mode extract-title-text
  (element (BRIDGEHEAD)
    (process-children)))

(element (BOOK TITLE)
  (make element
    gi: "h1"
    attributes: (list "align" "center")
  (process-children)))

(element (ARTICLE TITLE)
  (make element
    gi: "h1"
    attributes: (list "align" "center")
  (process-children)))

(element (CHAPTER TITLE)
  (make sequence
    (make element
      gi: "h1"
      (make sequence
        (literal (chap-app-head-label "Chapter"))
        (process-children-trim)))))

(element (PREFACE TITLE)
  (make element
    gi: "h1"
    (make sequence
      (literal "Preface: ")
      (process-children-trim))))

(element (APPENDIX TITLE)
  (make element
    gi: "H1"
    (make sequence
      (literal (chap-app-head-label "Appendix"))
      (process-children-trim))))

(element (BIBLIOGRAPHY TITLE)
  (make element gi: "H1"
	(make sequence
 	  (literal (chap-app-head-label "Bibliography"))
 	  (process-children-trim))))

(element (BOOKBIBLIO TITLE)
  (make element gi: "H2"
	(make sequence
;;;	  (literal (chap-app-head-label "Bibliography"))
	  (process-children-trim))))

(element (BIBLIODIV TITLE)
  (make element gi: "H2"
	(make sequence
	  (process-children-trim))))

(element (GLOSSARY TITLE)
  (make element gi: "H1"
	(make sequence
 	  (literal "Glossary")
; 	  (process-children-trim)
)))

(element (GLOSSDIV TITLE)
  (make element gi: "H2"
	(process-children-trim)))

(element (ARTHEADER TITLE)
  (make element gi: "H1"
	(process-children-trim)))

(element (ARTHEADER DATE)
  (make element gi: "P"))

(element (ARTHEADER SUBTITLE)
  (make element
    gi: "H2"
    (make element
      gi: "i"
      (process-children-trim)
      )))

(element (SECT1 TITLE) (make element gi: "H2"))

(element (SECT2 TITLE) (make element gi: "H3"))

(element (SECT3 TITLE) (make element gi: "H4"))

(element (SECT4 TITLE) (make element gi: "H5"))

(element (SECT5 TITLE) (make element gi: "H6"))

(element (FORMALPARA TITLE) (make element gi: "H6"))

(element BRIDGEHEAD (make element gi: "H4"))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; font changers

(element EMPHASIS
  (make element gi: "EM"))

(element TYPE
  (make element gi: "STRONG"
	(make element gi: "TT")))

(element TOKEN
  (make element gi: "EM"
	(make element gi: "STRONG"
	      (make element gi: "TT"))))

(element REPLACEABLE (make element gi: "EM"))

(element FIRSTTERM (make element gi: "EM"))

(element APPLICATION (make element gi: "TT"))

(element FILENAME (make element gi: "TT"))

(element LITERAL (make element gi: "TT"))

(element GUIMENU (make element gi: "TT"))
(element GUIMENUITEM (make element gi: "TT"))

(element ENVAR (make element gi: "TT"))

(element SUBSCRIPT (make element gi: "SUB"))

(element SUPERSCRIPT (make element gi: "SUP"))

(element CITATION (process-children))
(element CITETITLE (make element gi: "I"))

(element COMMAND (make element gi: "TT"))

(element STRUCTFIELD (make element gi: "TT"))

(element OPTION (make element gi: "TT"))

(element USERINPUT (make element gi: "TT"))

(element COMPUTEROUTPUT (make element gi: "TT"))

(element PROMPT (make element gi: "TT"))

(element PRODUCTNAME (make element gi: "I"))

(element SGMLTAG (make element gi: "TT"))
(element CLASSNAME (make element gi: "TT"))
(element SYMBOL (make element gi: "TT"))
(element LITERALLAYOUT (make element gi: "PRE"))
(element PROGRAMLISTING (make element gi: "PRE"))

(element FOREIGNPHRASE (make element gi: "I"))

;; I think that title abbreviations don't really have a place in HTML.
;; The only place in which I can imagine them being useful is if the
;; HTML secret title (the one that goes in your bookmarks and stuff)
;; is derived from the DocBook title abbrev
;;
;; (element ABBREV (process-children-trim))
(element ABBREV (empty-sosofo))
(element TITLEABBREV (empty-sosofo))

(element EMAIL
  (if %email-element%
      (make element
	gi: %email-element%
	(process-children-trim))
      (process-children-trim)))

(element QUOTE
  (make sequence
    (make entity-ref
      name: "quot")
    (process-children-trim)
    (make entity-ref
      name: "quot")))

(element ADDRESS
  (make element
    gi: "address"
    (make element
      gi: "em"
      (process-children-trim))))

(element (ADDRESS CITY)
  (make sequence
    (make empty-element
      gi: "BR")
    (process-children-trim)))

(element (ADDRESS COUNTRY)
  (make sequence
    (make empty-element
      gi: "BR")
    (process-children-trim)))

(element (ADDRESS EMAIL)
  (make sequence
    (make empty-element
      gi: "BR")
    ;; Now throw some space between street address and the email
    ;; address.  Sadly, the only way I know how to do it in HTML is to
    ;; throw in an empty paragraph tag.
    (make empty-element
      gi: "spacer"
      attributes: '(("align" "vertical") ("size" "10")))
    (if %email-element%
	(make element
	  gi: %email-element%
	  (process-children-trim))
	(process-children-trim))))

(element (ADDRESS FAX)
  (make sequence
    (make empty-element
      gi: "BR")
    (process-children-trim)))

(element (ADDRESS OTHERADDR)
  (make sequence
    (make empty-element
      gi: "BR")
    (process-children-trim)))

(element (ADDRESS POB)
  (make sequence
    (make empty-element
      gi: "BR")
    (process-children-trim)))

(element (ADDRESS PHONE)
  (make sequence
    (make empty-element
      gi: "BR")
    (process-children-trim)))

(element (ADDRESS POSTCODE)
  (process-children-trim))

(element (ADDRESS STATE)
  (process-children-trim))

(element (ADDRESS STREET)
  (make sequence
    (make empty-element
      gi: "BR")
    (process-children-trim)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; paragraph like things

(element CAUTION (make-special-para))

(element IMPORTANT (make-special-para))

(element WARNING (make-special-para))

(element NOTE (make-special-para))

(element TIP (make-special-para))

(element EXAMPLE (make-special-para))

(element INFORMALEXAMPLE
  (make element
    gi: "BLOCKQUOTE"))

(element COMMENT
  (cond (%show-comments%
	 (make element
	   gi: "FONT"
	   attributes: '(("color" "red"))
	   (make-special-para)))
	(#t (empty-sosofo))))

(element PARA
  (make sequence
    (make empty-element
      gi: "P")
    (with-mode footnote-ref
      (process-children))
    (with-mode footnote-def
      (process-matching-children "FOOTNOTE"))))

(element BLOCKQUOTE (make element gi: "BLOCKQUOTE"))

(element SCREEN
  (let ((gubbins (make element
		   gi: "PRE"
		   (process-children))))
    (make sequence
      (make empty-element
	gi: "P")
      (if %no-shade-screen%
	  gubbins
	  (make element
	    gi: "TABLE"
	    attributes: (append (list '("border" "0")
				      '("bgcolor" "#E0E0E0"))
				(if %shade-width%
				    (list (list "width" %shade-width%))
				    '()))
	    (make element
	      gi: "TR"
	      (make element
		gi: "TD"
		gubbins)))))))

(element FORMALPARA (process-children))

(element PHRASE (maybe-bold-children))

(mode footnote-ref
  (element FOOTNOTE
    (make sequence
      (literal "[")
      (literal (format-number (element-number (current-node)) "1"))
      (literal "]"))))

(mode footnote-def
  (element FOOTNOTE
    (make element
      gi: "BLOCKQUOTE"
      (make sequence
	(literal "[")
	(literal (format-number (element-number (current-node)) "1"))
	(literal "]")
	(process-children)))))

(element (CAUTION TITLE)
  (make element
      gi: "H5"))

(element (IMPORTANT TITLE)
  (make element
      gi: "H5"))

(element (WARNING TITLE)
  (make element
      gi: "H5"))

(element (NOTE TITLE)
  (make element
      gi: "H5"))

(element (TIP TITLE)
  (make element
      gi: "H5"))

(element (EXAMPLE TITLE)
  (make element
      gi: "H5"))

(element (BIBLIOENTRY TITLE)
  (make element
      gi: "H3"))

(element (BIBLIOENTRY PAGENUMS)
  (make sequence
	(literal "Pages: ")
	(make element gi: "I")))

(element (BIBLIOENTRY DATE)
  (make sequence
	(literal " Date: ")
	(make element gi: "I")))

(element (BIBLIOENTRY VOLUMENUM)
  (make sequence
	(literal " Volume: ")
	(make element gi: "I")))

(element (BIBLIOENTRY SERIESINFO) (process-children))

(element (BIBLIOENTRY SERIESINFO TITLE)
	 (make element gi: "H4"))

(element (BIBLIOENTRY BIBLIOMISC)
  (make sequence
	(literal " other: ")
	(process-children)))

; the para inside does the work, here...
(element (BIBLIOENTRY ABSTRACT) (process-children))

;; an article'sabstract is centered text
(element ABSTRACT
  (make sequence
    (make empty-element gi: "HR")
    (make element
      gi: "H2"
      (literal "Abstract"))
    (make element
      gi: "DIV"
      attributes: (list (list "align" "left"))
      (make element gi: "BLOCKQUOTE"
	    (process-children)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; lists

(element ITEMIZEDLIST
  (make sequence
    (make empty-element
      gi: "P")
    (make element
      gi: "UL")))

(element ORDEREDLIST
  (make sequence
    (make empty-element
      gi: "P")
    (make element
      gi: "OL")))

(element PROCEDURE
  (make sequence
    (make empty-element
      gi: "P")
    (make element
      gi: "OL")))

(element SIMPLELIST
  (make sequence
    (make empty-element
      gi: "P")
    (make element
      gi: "UL")))

(element (ITEMIZEDLIST LISTITEM)
  (make sequence
    (make empty-element
      gi: "LI")
    (process-children)
    (make empty-element
      gi: "P")))

(element (ORDEREDLIST LISTITEM)
  (make sequence
    (make empty-element
      gi: "LI")
    (process-children)
    (make empty-element
      gi: "P")))

(element STEP
  (make sequence
    (make empty-element
      gi: "LI")
    (process-children)
    (make empty-element
      gi: "P")))

(element (SIMPLELIST MEMBER)
  (make sequence
    (make empty-element
      gi: "LI")
    (process-children)))

(element VARIABLELIST
  (make sequence
    (make empty-element
      gi: "P")
    (make element
      gi: "DL")))

(element VARLISTENTRY
  (make sequence
    (make empty-element
      gi: "DT")
    (process-children)))

(element (VARLISTENTRY LISTITEM)
  (make sequence
    (make empty-element
      gi: "DD")
    (process-children)
    (make empty-element
      gi: "P")))

(element TERM (maybe-bold-children))

(element GLOSSDIV
  (make sequence
    (make empty-element
      gi: "P")
    (make element
      gi: "DL")))

; (define (gloss-entry-name indexterm)
;   (string-append "gloss." (format-number (element-number indexterm) "1")))
(define (gloss-entry-name glossterm)
  (string-append "gloss." (data glossterm)))

(element GLOSSENTRY
  (process-children))

(element (GLOSSENTRY GLOSSTERM)
  (make sequence
    (make empty-element
      gi: "DT")
    (make empty-element
      gi: "A"
      attributes: (list (list "name" (gloss-entry-name (current-node)))))
    (process-children)))


(element (GLOSSENTRY GLOSSDEF)
  (make sequence
    (make empty-element
      gi: "DD")
    (process-children)
    (make empty-element
      gi: "P")))

(element (GLOSSENTRY GLOSSSEE)
  (make sequence
    (make empty-element
      gi: "DD")
    (literal "See ")
    (make element
      gi: "A"
      attributes: (list (list "href"
			      (string-append "#"
					     (gloss-entry-name (current-node)))
			      )))
    (make empty-element
      gi: "P")))

; (element GLOSSTERM (maybe-bold-children))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; index

(define (index-entry-name indexterm)
  (string-append "index." (format-number (element-number indexterm) "1")))

(element INDEXTERM
  (make sequence
    (make element
      gi: "A"
      attributes: (list (list "name" (index-entry-name (current-node))))
      (literal ""))
    (empty-sosofo)))

; DIY string-ci>?

(define (string-ci>? s1 s2)
  (let ((len1 (string-length s1))
	(len2 (string-length s2)))
    (let loop ((i 0))
      (cond ((= i len1) #f)
	    ((= i len2) #t)
	    (#t (let ((c1 (index-char-val (string-ref s1 i)))
		      (c2 (index-char-val (string-ref s2 i))))
		  (cond
		   ((= c1 c2) (loop (+ i 1)))
		   (#t (> c1 c2)))))))))

(define (index-char-val ch)
  (case ch
    ((#\A #\a) 65)
    ((#\B #\b) 66)
    ((#\C #\c) 67)
    ((#\D #\d) 68)
    ((#\E #\e) 69)
    ((#\F #\f) 70)
    ((#\G #\g) 71)
    ((#\H #\h) 72)
    ((#\I #\i) 73)
    ((#\J #\j) 74)
    ((#\K #\k) 75)
    ((#\L #\l) 76)
    ((#\M #\m) 77)
    ((#\N #\n) 78)
    ((#\O #\o) 79)
    ((#\P #\p) 80)
    ((#\Q #\q) 81)
    ((#\R #\r) 82)
    ((#\S #\s) 83)
    ((#\T #\t) 84)
    ((#\U #\u) 85)
    ((#\V #\v) 86)
    ((#\W #\w) 87)
    ((#\X #\x) 88)
    ((#\Y #\y) 89)
    ((#\Z #\z) 90)

    ((#\ ) 32)

    ((#\0) 48)
    ((#\1) 49)
    ((#\2) 50)
    ((#\3) 51)
    ((#\4) 52)
    ((#\5) 53)
    ((#\6) 54)
    ((#\7) 55)
    ((#\8) 56)
    ((#\9) 57)

    ; laziness precludes me from filling this out further
    (else 0)))

(define (string->number-list s)
  (let loop ((i (- (string-length s) 1))
	     (l '()))
    (if (< i 0)
	l
	(loop (- i 1) (cons (index-char-val (string-ref s i)) l)))))

(define (number-list>? l1 l2)
  (cond ((null? l1) #f)
	((null? l2) #t)
	((= (car l1) (car l2))
	 (number-list>? (cdr l1) (cdr l2)))
	(#t (> (car l1) (car l2)))))

; return the string data for a given index entry

(define (get-index-entry-data entry)
  (let ((primary (select-elements (descendants entry) "PRIMARY"))
	(secondary (select-elements (descendants entry) "SECONDARY")))
    (if (node-list-empty? secondary)
	(data primary)
	(string-append (data primary) " - " (data secondary)))))

(define (make-index-entry entry)
  (let ((text (get-index-entry-data entry)))
    (cons text
	  (make sequence
	    (make empty-element
	      gi: "LI")
	    (make element
	      gi: "A"
	      attributes: (list (list "href"
				      (string-append (link-file-name
						      entry)
						     "#"
						     (index-entry-name
						      entry))))
	      (literal text))))))

(define (build-index nl)
  (let loop ((result '())
	     (nl nl))
    (if (node-list-empty? nl)
	result
	(loop (cons (make-index-entry (node-list-first nl)) result)
	      (node-list-rest nl)))))

(define (sort-index il)
  (letrec ((list-head (lambda (l n)
			(if (> n 0)
			    (cons (car l) (list-head (cdr l) (- n 1)))
			    '())))
	   (merge (lambda (il1 il2)
		    (cond ((null? il1) il2)
			  ((null? il2) il1)
			  ((string-ci>? (car (car il1)) (car (car il2)))
			   (cons (car il2) (merge il1 (cdr il2))))
			  (#t
			   (cons (car il1) (merge (cdr il1) il2)))))))
    (let* ((ll (length il))
	   (ldiv2 (quotient ll 2)))
      (if (> 2 ll)
	  il
	  (merge (sort-index (list-head il ldiv2))
		 (sort-index (list-tail il ldiv2)))))))

(define (output-index il)
  (let extract-and-append ((il il)
			   (result (empty-sosofo)))
    (if (null? il)
	result
	(extract-and-append (cdr il) (sosofo-append result (cdr (car il)))))))

(define (make-index)
  (make sequence
    (make element
      gi: "A"
      attributes: (list (list "name" "INDEXTOP"))
      (literal ""))
    (make element
      gi: "H1"
      (literal "Index"))
    (make element
      gi: "UL"
      (output-index
       (sort-index
	(build-index (select-elements (descendants (current-node))
					   "INDEXTERM")))))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; links & cross-references

(define (link-file-name target)
  (cond (%no-split-output% "")
	(#t
	 (string-append
	  %output-basename%
	  (cond ((equal? (gi target) "CHAPTER")
		 (string-append
		  "-"
		  (format-number (child-number target) "1")))
		((ancestor-child-number "CHAPTER" target)
		 (string-append
		  "-"
		  (format-number (ancestor-child-number "CHAPTER" target) "1")))
		((equal? (gi target) "APPENDIX")
		 (string-append
		  "-"
		  (format-number (child-number target) "A")))
		((ancestor-child-number "APPENDIX" target)
		 (string-append
		  "-"
		  (format-number (ancestor-child-number "APPENDIX" target) "A")))
		(#t ""))
	  %output-suffix%))))

(element ANCHOR
  (make-anchor))

(element LINK
  (let* ((target (element-with-id (attribute-string "linkend")
				  (book-node)))
	 (target-file-name (link-file-name target))
	 (endterm (attribute-string "endterm"))
	 (target-title-sosofo (if endterm
				  (with-mode extract-xref-text
					     (process-node-list
					      (element-with-id endterm
							       (book-node))))
				  (process-children))))
    (make element
	  gi: "A"
	  attributes: (list
		       (list "href"
			     (string-append
			      target-file-name
			      "#"
			      (attribute-string "linkend"))))
	  target-title-sosofo)))
(element ULINK
  (make element
    gi: "A"
    attributes: (list
		 (list "href" (attribute-string "url")))))

(element XREF
  (let* ((target (element-with-id (attribute-string "LINKEND")
				  (book-node)))
	 (target-file-name (link-file-name target)))
    (make element
      gi: "A"
      attributes: (list
		   (list "href"
			 (string-append target-file-name
					"#"
					(attribute-string "linkend"))))
      (with-mode extract-xref-text
	(process-node-list target)))))

(mode extract-xref-text
  (default
    (let ((title-sosofo (with-mode extract-title-text
			  (process-first-descendant "TITLE"))))
      (if (sosofo? title-sosofo)
	  title-sosofo
	  (literal (string-append "Reference to " (gi)))))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; figures

(element FIGURE
  (make sequence
    (make empty-element
      gi: "P")
    (make-anchor)
    (process-children)
    (make empty-element
      gi: "P")))

(element (FIGURE TITLE)
  (make sequence
    (make element
      gi: "B")
    (make empty-element
      gi: "P")))

(element GRAPHIC
  (let ((img
	 (make sequence
	   (make empty-element
	     gi: "P")
	   (make empty-element
	     gi: "IMG"
	     attributes: (list
			  (list "src"
				(string-append (attribute-string "fileref")
					       ".gif")))))))
    (if (equal?
	 (attribute-string "align")
	 "CENTER")
	(make element
	  gi: "CENTER"
	  img)
	img)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; tables

(element TABLE
  (make sequence
    (make empty-element
      gi: "P")
    (make element
      gi: "TABLE"
      attributes: (if (equal?
		       (attribute-string "frame")
		       "ALL")
		      '(("border" "2") ("cellpadding" "2"))
		      '()))
    (make empty-element
      gi: "P")))

(element (TABLE TITLE)
  (make element
    gi: "H4"
    (make sequence
     (literal "Table: ")
     (process-children-trim))))

(element INFORMALTABLE
  (make sequence
    (make empty-element
      gi: "P")
    (make element
      gi: "TABLE"
      attributes: (if (equal?
		       (attribute-string "frame")
		       "ALL")
		      '(("border" "2") ("cellpadding" "2"))
		      '()))
    (make empty-element
      gi: "P")))

(element TGROUP (process-children))

(element THEAD (process-children))

(element (THEAD ROW)
  (make sequence
    (make empty-element
      gi: "TR")
    (process-children)))

(element (THEAD ROW ENTRY)
  (make sequence
    (make empty-element
      gi: "TD")
    (make element
      gi: "B"
      (process-children))))

(element TBODY (process-children))

(element (TBODY ROW)
  (make sequence
    (make empty-element
      gi: "TR")
    (process-children)))


(element (TBODY ROW ENTRY)
  (make sequence
    (make empty-element
      gi: "TD")
    (process-children)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; book info

(element BOOKINFO
  (make sequence
    (make element
      gi: "CENTER"
      (process-children))
    (cond ((not %no-make-toc%)
	   (make sequence
	     (make-fat-rule)
	     (make element
	       gi: "H2"
	       (literal "Contents"))
	     (make element
	       gi: "ul"
	       (with-mode make-toc-links
		 (process-node-list (book-node))))))
	  (#t (empty-sosofo)))))

(element (BOOKINFO DATE)
  (make element gi: "P"))

(element (BOOKINFO TITLE)
  (make element gi: "H1"))

(element (BOOKINFO SUBTITLE)
  (make element
    gi: "H2"
    (make element
      gi: "i"
      (process-children-trim)
      )))

(element AUTHORGROUP
  (let ((reducer (lambda (sofar new)
		   (sosofo-append sofar (make element
					  gi: "H2"
					  (process-node-list new))))))
    (make sequence
      (node-list-reduce (select-elements (descendants (current-node)) "AUTHOR")
			reducer
			(empty-sosofo))
      (node-list-reduce (select-elements (descendants (current-node)) "EDITOR")
			reducer
			(empty-sosofo))
      (node-list-reduce (select-elements (descendants (current-node)) "CORPAUTHOR")
			reducer
			(empty-sosofo)))))

;; I make a new left-aligned DIV sandwiched between HRs for the author
;; blurb.
(element AUTHORBLURB
  (make sequence
    (make empty-element gi: "HR")
    (make element
      gi: "DIV"
      attributes: '(("align" "left"))
      (make element
	gi: "h4"
	attributes: '(("align" "left"))
	(literal "Author blurb for ")
	(make element
	  gi: "i"
	  (process-matching-children "TITLE")
	  (literal ":")))
      (process-matching-children "FORMALPARA" "PARA" "SIMPARA"))
    (make empty-element gi: "HR")))

(element (AUTHORBLURB TITLE)
  (make element gi: "B"))

(element CORPNAME
  (make element gi: "H2"))

(element (BIBLIOENTRY AUTHORGROUP)
  (let ((reducer (lambda (sofar new)
		   (sosofo-append sofar (make element
					  gi: "H3"
					  (process-node-list new))))))
    (make sequence
      (node-list-reduce (select-elements (descendants (current-node)) "AUTHOR")
			reducer
			(empty-sosofo))
      (node-list-reduce (select-elements (descendants (current-node)) "EDITOR")
			reducer
			(empty-sosofo))
      (node-list-reduce (select-elements (descendants (current-node)) "CORPAUTHOR")
			reducer
			(empty-sosofo)))))

(element COPYRIGHT
  (make element
    gi: "H4"
    (make sequence
      (make entity-ref
	name: "copy")
      (process-matching-children "HOLDER")
      (process-matching-children "YEAR"))))

(element HOLDER
  (make sequence
    (literal " ")
    (process-children-trim)))

(element YEAR
  (make sequence
    (literal " ")
    (process-children-trim)))

(element CORPAUTHOR
  (process-children-trim))

(element AUTHOR
  (process-children-trim))

(element EDITOR
  (process-children-trim))

(element CONFGROUP
  (process-children-trim))

(element CONFTITLE
  (make sequence
    (make empty-element
      gi: "BR")
    (make element gi: "I" (process-children))))

(element CONFDATES
  (make sequence
    (make empty-element
      gi: "BR")
    (process-children)))

(element HONORIFIC
  (make sequence
    (process-children-trim)
    (literal " ")))

(element FIRSTNAME
  (make sequence
    (process-children-trim)
    (literal " ")))

(element OTHERNAME
  (make sequence
    (process-children-trim)
    (literal " ")))

(element SURNAME
  (make sequence
    (process-children-trim)
    (literal " ")))

(element LINEAGE
  (make sequence
    (process-children-trim)
    (literal " ")))

(element TRADEMARK (process-children))

(element PUBLISHERNAME (process-children))

(element BIBLIOENTRY (process-children))

(element ACRONYM (process-children))

(element RELEASEINFO
  (make sequence
    (make empty-element
      gi: "BR")
    (make element gi: "B")))

(element AFFILIATION
  (make sequence
    (make element
      gi: "I")))

(element ORGNAME
  (make sequence
    (make empty-element
      gi: "BR")
    (process-children)))

;;(element ORGNAME
;;    (process-children))

(element JOBTITLE
  (make sequence
    (make empty-element
      gi: "BR")
    (process-children)))

(element ORGDIV
  (make sequence
    (make empty-element
      gi: "BR")
    (process-children)))

(element PUBLISHER
  (make sequence
    (make empty-element
      gi: "BR")
    (process-children)))

(element ISBN
  (make sequence
    (make empty-element
      gi: "BR")
    (process-children)))

(element PUBDATE
  (make sequence
    (make empty-element
      gi: "BR")
    (process-children)))

(element REVHISTORY
  (empty-sosofo))

(element LEGALNOTICE
  (make sequence
    (make element
      gi: "H4"
      attributes: '(("align" "left"))
      (literal "Legal Notice: "))
    (make element
      gi: "DIV"
      attributes: '(("align" "left")))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; Synopses for command line, API functions and so forth
;;(element FUNCSYNOPSIS
;;  (process-children))

(element FUNCSYNOPSIS
  (make sequence
    (make empty-element gi: "P")	; go to a new line
    ;; just in case there is some synopsisinfo, like #include directives
    (process-matching-children "FUNCSYNOPSISINFO")
    (make empty-element gi: "P")	; go to a new line
    (process-matching-children "FUNCDEF") ; this gives the function name
    (literal " (")
    (process-matching-children "PARAMDEF") ; this gives the function arguments
    (literal ")")
    ))

(element FUNCDEF
  (make element gi: "TT"))

(element FUNCTION
  (make element gi: "B"))

(element STRUCTNAME
  (make element gi: "B"))

(element FUNCSYNOPSISINFO
  (make element
    gi: "PRE"))

(element PARAMDEF
  (make element
      gi: "TT"))

(element PARAMETER
  (make element
    gi: "I"))

(element FUNCPROTOTYPE
  (make element
    gi: "PRE"))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; TOC

(element LOF (empty-sosofo))

(element LOT (empty-sosofo))

(element TOC (empty-sosofo))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; DIY TOC

(mode make-toc-links
  (element (BOOK)
    (sosofo-append
     (process-children)
     (cond ((not %no-make-index%)
	    (make sequence
	      (make empty-element
		gi: "LI")
	      (make element
		gi: "A"
		attributes: (list (list "href"
					(cond (%no-split-output% "#INDEXTOP")
					      (#t
					       (string-append %output-basename%
							      "-INDEX"
							      %output-suffix%
							      "#INDEXTOP")))))
		(literal "Index"))))
	   (#t (empty-sosofo)))))
  (element (CHAPTER)
    (make-chap-or-app-toc-links))
  (element (APPENDIX)
    (make-chap-or-app-toc-links))
  (element (SECT1)
    (make sequence
      (make empty-element
	gi: "LI")
      (let ((title-text (with-mode extract-title-text
			  (process-first-descendant "TITLE"))))
	(if (id)
	    (make element
	      gi: "A"
	      attributes: (list (list "href" (string-append (link-file-name (current-node))
							    "#"
							    (id))))
	    title-text)
	    title-text))))
  (default
    (empty-sosofo)))

(define (make-chap-or-app-toc-links)
  (make sequence
    (make empty-element
      gi: "LI")
    (let ((title-text
	   (make sequence
	     (literal (if (equal? (gi) "CHAPTER")
			  (string-append "Chapter "
					 (format-number
					  (element-number (current-node))
					  "1")
					 " - ")
			  (string-append "Appendix "
					 (format-number
					  (element-number (current-node))
					  "A")
					 " - ")))
	     (with-mode extract-title-text
	       (process-first-descendant "TITLE")))))
      (if (id)
	  (make element
	    gi: "A"
	    attributes: (list (list "href" (string-append (link-file-name (current-node))
									  "#"
									  (id))))
	    title-text)
	  title-text))
    (make element
      gi: "UL"
      (with-mode make-toc-links
	(process-matching-children "SECT1")))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; make the unimplemented bits stand out

(default
  (make element
    gi: "FONT"
    attributes: '(("color" "red"))
    (make sequence
      (literal (string-append "<" (gi) ">"))
      (process-children)
      (literal (string-append "</" (gi) ">")))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; standard subroutines

(define (node-list-reduce nl combine init)
  (if (node-list-empty? nl)
      init
      (node-list-reduce (node-list-rest nl)
                        combine
                        (combine init (node-list-first nl)))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; various homebrew subroutines

(define (book-node)
  (cond ((equal? (gi) "BOOK") (current-node))
	(#t (let ((book-root (ancestor "BOOK")))
	      (if (node-list-empty? book-root)
		  (cond ((equal? (gi) "CHAPTER") (current-node))
			(#t (ancestor "CHAPTER")))
		  book-root)))))

(define (make-fat-rule)
  (make sequence
    (make empty-element gi: "P")
    (make empty-element gi: "HR")))

(define (chap-app-head-label chap-or-app)
  (let ((label
	 (attribute-string "label" (ancestor chap-or-app))))
    (string-append
     chap-or-app
     " "
     (if label
	 (if (equal? label "auto")
	     (format-number
	      (element-number (ancestor chap-or-app))
	      (if (equal? chap-or-app "Chapter") "1" "A"))
	   label)
       (format-number
	(element-number (ancestor chap-or-app))
	(if (equal? chap-or-app "Chapter") "1" "A")))
     ". ")))

(define (make-anchor)
  (if (id)
      (make element
	gi: "A"
	attributes: (list (list "name" (id)))
	(literal ""))
      (empty-sosofo)))

(define (make-special-para)
  (make sequence
    (make empty-element
      gi: "P")
    (make element
      gi: "B"
      (literal (string-append (gi) ":")))
    (make element
      gi: "BLOCKQUOTE"
      (process-children))))

(define (maybe-bold-children)
  (cond ((equal? (attribute-string "role")
		 "bold")
	 (make element
	   gi: "B"
	   (process-children-trim)))
	(#t (process-children-trim))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; the end
