;; $Id$

(define cpu-type '(("name"	. "cpu")
		   ("label"	. "CPU Usage")
		   ("fields"	. (list
				   (("name"	. "total")
				    ("label"	. "Total CPU Usage")
				    ("type"	. "unsigned long"))
				   (("name"	. "user")
				    ("type"	. "unsigned long"))
				   (("name"	. "nice")
				    ("type"	. "unsigned long"))
				   (("name"	. "sys")
				    ("type"	. "unsigned long"))
				   (("name"	. "idle")
				    ("type"	. "unsigned long"))
				   (("name"	. "name")
				    ("type"	. "const char")
				    ("pointer"	. #t))
				   (("name"	. "test"))
				   )
				)
		   )
  )

(define main-function
  (lambda (definition)
    (letrec ((default-type "unsigned long")
	     (struct-label-comments #t)
	     (struct-name-comments #f)
	     (default-name-tabs-first 3)
	     (default-name-tabs 4)
	     (default-type-tabs 2)
	     (default-comment-tabs 5)
	     (default-definition-tabs 3)
	     (default-definition-value-tabs 2)
	     (default-max-line-length 60)
	     ;; set default values for unspecified fields
	     (check-field-definition
	      (lambda (fields)
		(let ((newlist (list)))
		  (for-each
		   (lambda (field)
		     (if (not (assoc-ref field "type"))
			 (set! field (assoc-set! field "type" default-type)))
		     (if (assoc-ref field "label")
			 (set! field (assoc-set! field "has-label" #t))
			 (begin
			   (set! field (assoc-set! field "label" (assoc-ref field "name")))
			   (set! field (assoc-set! field "has-label" #f))
			   )
			 )
		     (set! newlist (append newlist (list field)))
		     )
		   (cdr fields))
		  (set-cdr! fields newlist)
		  )
		)
	      )
	     ;; number fields sequentially
	     (make-field-numbers
	      (lambda (fields)
		(let ((pos 0) (newlist (list)))
		  (for-each
		   (lambda (field)
		     (set! field (assoc-set! field "number" pos))
		     (set! newlist (append newlist (list field)))
		     (set! pos (+ pos 1))
		     )
		   (cdr fields))
		  (set-cdr! fields newlist)
		  (set! definition (assoc-set! definition "max-fields" pos))
		  )
		)
	      )
	     ;; pad string with up to 'tabs' tabs
	     (tabify-string
	      (lambda (string tabs)
		(let ((length (string-length string))
		      (tlength (* tabs 8)))
		  (if (> tlength length)
		      (let* ((diff (- tlength length))
			     (count (quotient (+ diff 7) 8)))
			(string-append string
				       (make-string count #\tab))
			)
		      (string-append string
				     #\space)
		      )
		  )
		)
	      )
	     ;; pad string with spaces
	     (spacify-string
	      (lambda (string tabs)
		(let ((length (string-length string))
		      (tlength (* tabs 8)))
		  (if (> tlength length)
		      (string-append string
				     (make-string (- tlength length) #\space))
		      (string-append string
				     #\space)
		      )
		  )
		)
	      )
	     ;; creates comment string
	     (make-comment-string
	      (lambda (comment)
		(if comment
		    (string "/* "
			    (spacify-string comment
					    default-comment-tabs)
			    " */")
		    (string)
		    )
		)
	      )
	     ;; create constant for entry (eg. GLIBTOP_CPU_TOTAL)
	     (entry-constant-name
	      (lambda (name)
		(string-upcase! (string "GLIBTOP_"
					(assoc-ref definition "name")
					"_"
					name
					)
				)
		)
	      )
	     ;; create text that is displayed as comment along with entry
	     (entry-comment-text
	      (lambda (name label)
		(if label
		    (if struct-label-comments label #f)
		    (if struct-name-comments (entry-constant-name name) #f)
		    )
		)
	      )
	     ;; starts struct definition
	     (make-struct-definition-head
	      (lambda ()
		(let* ((class (assoc-ref definition "name"))
		       )
		  (string "struct _glibtop_" class "\n"
			  "{\n"
			  )
		  )
		)
	      )
	     ;; terminates struct definition
	     (make-struct-definition-tail
	      (lambda ()
		(string "};\n\n")
		)
	      )
	     ;; generate struct definition body
	     (make-struct-definition-body
	      (lambda (fields)
		(letrec ((output (string))
			 (is-first-entry #t)
			 (current-type #f)
			 (current-name #f)
			 (current-label #f)
			 (current-pointer #f)
			 ;; close entry (other = next entry is of other type)
			 (entry-end
			  (lambda (other)
			    (let ((old-first is-first-entry))
			      (set! is-first-entry other)
			      (if current-type
				  (string (tabify-string (string (string (if current-pointer "*" ""))
								 current-name
								 (string (if other ";" ","))
								 )
							 (if old-first
							     default-name-tabs-first
							     default-name-tabs
							     )
							 )
					  (make-comment-string (entry-comment-text
								current-name current-label))
					  "\n")
				  (string)
				  )
			      )
			    )
			  )
			 ;; start new entry
			 (entry-start
			  (lambda (name type)
			    (if current-type
				(if (equal? current-type type)
				    (string (entry-end #f) "\t\t")
				    (string (entry-end #t) "\t"
					    (tabify-string type default-type-tabs)
					    )
				    )
				(string "\t"
					(tabify-string type default-type-tabs)
					)
				)
			    )
			  )
			 )
		  ;; main function
		  (for-each
		   (lambda (field)
		     (let ((name (assoc-ref field "name"))
			   (type (assoc-ref field "type"))
			   (pointer (assoc-ref field "pointer"))
			   (label (if (assoc-ref field "has-label")
				      (assoc-ref field "label")
				      #f)
				  )
			   )
		       (set! output (string-append output
						   (string (entry-start name type))
						   )
			     )
		       ;; save current data
		       (set! current-type type)
		       (set! current-name name)
		       (set! current-label label)
		       (set! current-pointer pointer)
		       )
		     )
		   (cdr fields))
		  ;; close last entry
		  (string-append output (entry-end #t))
		  )
		)
	      )
	     ;; display complete struct definition
	     (make-struct-definition
	      (lambda (fields)
		(string-append (make-struct-definition-head)
			       (make-struct-definition-body fields)
			       (make-struct-definition-tail)
			       )
		)
	      )
	     ;; make constant definition
	     (make-const-definition
	      (lambda (name value comment)
		(let* ((nstring  (string-upcase! (string "GLIBTOP_" name)))
		       (tabname  (tabify-string nstring default-definition-tabs))
		       (tabvalue (if comment
				     (tabify-string (string value) default-definition-value-tabs)
				     (string value))
				 )
		       (ctext    (make-comment-string comment))
		       (line     (string "#define " tabname tabvalue ctext "\n"))
		       )
		  line)
		)
	      )
	     ;; creates constant definitions
	     (make-struct-constants
	      (lambda (fields)
		(let ((class  (assoc-ref definition "name"))
		      (output (string))
		      )
		  (for-each
		   (lambda (field)
		     (let* ((name   (assoc-ref field "name"))
			    (number (assoc-ref field "number"))
			    (key    (string class "_" name))
			    (value  (number->string number))
			    (label  (if (assoc-ref field "has-label")
					(assoc-ref field "label")
					#f)
				    )
			    (line   (make-const-definition key
							   value 
							   (entry-comment-text name label)
							   )
				    )
			    )
		       (set! output (string-append output line))
		       )
		     )
		   (cdr fields))
		  output)
		)
	      )
	     ;; creates definition of maximum constant
	     (make-struct-max-constant
	      (lambda ()
		(let* ((class (assoc-ref definition "name"))
		       (max   (assoc-ref definition "max-fields"))
		       )
		  (make-const-definition (string "MAX_" class)
					 (number->string max)
					 #f)
		  )
		)
	      )
	     ;; adds new list element to string, inserting newline if necessary
	     (add-to-string
	      (lambda (output line element separator indent max-length)
		(let* ((slen (string-length line))
		       (elen (string-length element))
		       (tlen (+ slen elen))
		       (bsep (if separator 
				 (string separator " ")
				 (string)
				 )
			     )
		       (nsep (string (if separator separator "") "\n"))
		       )
		  (if (and (> slen 0) (> tlen max-length))
		      (begin
			(set! output (string-append output line nsep))
			(set! line (string indent element))
			)
		      (set! line (string-append line bsep element))
		      )
		  (list output line)
		  )
		)
	      )
	     ;; create general list definition
	     (make-general-list-definition
	      (lambda (fields name symbol line-length make-element-string)
		(letrec ((class (assoc-ref definition "name"))
			 (make-general-list-definition-head
			  (lambda ()
			    (string "const char *"
				    "glibtop_"
				    symbol
				    "_"
				    class
				    " "
				    "[GLIBTOP_MAX_"
				    (string-upcase! (string class))
				    "] = \n{ "
				    )
			    )
			  )
			 (make-general-list-definition-tail
			  (lambda ()
			    (string "\n};\n")
			    )
			  )
			 (make-general-list-definition-body
			  (lambda ()
			    (let* ((first #t) (output (string)) (line (string)))
			      (for-each
			       (lambda (field)
				 (let* ((element (assoc-ref field name))
					(olist   (add-to-string output line
								(make-element-string element)
								(if first #f ",")
								"  "
								line-length))
					)
				   (set! output (car olist))
				   (set! line (car (cdr olist)))
				   (set! first #f)
				   )
				 )
			       (cdr fields))
			      (set! output (string-append output line))
			      output)
			    )
			  )
			 )
		  ;; main function
		  (string-append (make-general-list-definition-head)
				 (make-general-list-definition-body)
				 (make-general-list-definition-tail)
				 )
		  )
		)
	      )
	     ;; create name list definition
	     (make-name-list-definition
	      (lambda (fields)
		(make-general-list-definition fields
					      "name"
					      "names"
					      default-max-line-length
					      (lambda (x)
						(string #\" x #\")
						)
					      )
		)
	      )
	     ;; create label list definition
	     (make-label-list-definition
	      (lambda (fields)
		(make-general-list-definition fields
					      "label"
					      "labels"
					      0
					      (lambda (x)
						(string "gettext_noop (" #\" x #\" ")")
						)
					      )
		)
	      )
	     ;; create description list definition
	     (make-description-list-definition
	      (lambda (fields)
		(make-general-list-definition fields
					      "description"
					      "descriptions"
					      default-max-line-length
					      (lambda (x)
						(if x
						    (string "gettext_noop (" #\" x #\" ")")
						    (string "NULL")
						    )
						)
					      )
		)
	      )
	     )
      ;; start of main function
      (let ((fielddef (assoc-ref definition "fields")))
	(display fielddef) (newline) (newline)
	(check-field-definition fielddef)
	(make-field-numbers fielddef)
	(display fielddef) (newline) (newline)
	(display (make-struct-definition fielddef))
	(display (make-struct-constants fielddef)) (newline)
	(display (make-struct-max-constant)) (newline) (newline)
	(display (make-name-list-definition fielddef)) (newline)
	(display (make-label-list-definition fielddef)) (newline)
	(display (make-description-list-definition fielddef)) (newline)
	)
      )
    )
  )

(begin
  (main-function cpu-type)
  (newline)
  )

