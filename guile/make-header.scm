(define cpu-type '(("name" . "cpu")
		   ("label" . "CPU Usage")
		   ("fields" . (("unsigned long" ("total" "Total CPU Usage")
						 ("user") ("nice") ("sys") ("idle"))
				("const char" ("name"))
				)
			     )
		   )
  )

;; computes constant for struct field (eg. GLIBTOP_CPU_TOTAL)

(define field-name-constant
  (lambda (name field)
    (string "GLIBTOP_"
	    (string-upcase! (string name))
	    "_"
	    (string-upcase! (string field)))
    )
  )

;; computes structure name (eg. glibtop_cpu)

(define make-struct-name
  (lambda (type)
    (string "glibtop_" (assoc-ref type "name"))
    )
  )

(define tab-pad-string
  (lambda (string tabs)
    (string-append string (make-string (- (* tabs 8) (string-length string)) #\space))
    )
  )

(define make-field-list
  (lambda (name type fields)
    (let* ((output (string)) (pos 1))
      (map
       (lambda (x)
	 (let* ((sep   (if (= pos (length fields)) ";" ","))
		(start (if (= pos 1)
			   (string "\t"
				   (tab-pad-string (string type) 2))
			   (string "\t\t"))
		       )
		(comment (string (if (= pos 1) "" "\t") "/* "
				 (tab-pad-string (field-name-constant name (car x)) 3)
				 " */"))
		(field (tab-pad-string (string-append (string (car x)) sep) 2))
		)
	   (set! pos (+ pos 1))
	   (string-append start field comment "\n")
	   )
	 )
       fields)
      )
    )
  )

(define make-struct-body
  (lambda (type)
    (let* ((name   (assoc-ref type "name"))
	   (data   (assoc-ref type "fields"))
	   (output (string))
	   )
      (for-each
       (lambda (y)
	 (for-each
	  (lambda (z)
	    (set! output (string-append output z))
	    )
	  y)
	 )
       (map
	(lambda (x) (make-field-list name (car x) (cdr x)))
	data)
       )
      output)
    )
  )

(define make-struct
  (lambda (type)
    (let* ((name   (assoc-ref type "name"))
	   (data   (assoc-ref type "fields"))
	   (output (string-append
		    (tab-pad-string
		     (string "typedef struct _glibtop_" name)
		     5)
		    (string "glibtop_" name ";\n\n"
			    "struct glibtop_" name "\n{\n\t"
			    "unsigned long\tflags;\n")
		    )
		   )
	   )
      (string-append output (make-struct-body type) "};\n")
      )
    )
  )

(define make-field-name-list
  (lambda (type)
    (let* ((name   (assoc-ref type "name"))
	   (data   (assoc-ref type "fields"))
	   (return (list))
	   )
      (map
       (lambda (x)
	 (map
	  (lambda (y) (set! return (append return (list (car y)))))
	  (cdr x)
	  )
	 )
       data)
      return)
    )
  )

(define make-field-constants
  (lambda (type)
    (let* ((name   (assoc-ref type "name"))
	   (data   (make-field-name-list type))
	   (output (string
		    (tab-pad-string
		     (string "#define GLIBTOP_MAX_"
			     (string-upcase! (string name))
			     ) 5)
		    (number->string (length data))
		    "\n\n"
		    )
		   )
	   (pos 0)
	   )
      (for-each
       (lambda (x)
	 (set! output (string-append output (string
					     (tab-pad-string
					      (string "#define GLIBTOP_"
						      (string-upcase! (string name))
						      "_"
						      (string-upcase! (string x))
						      ) 5)
					     (number->string pos)
					     "\n"
					     )
				     )
	       )
	 (set! pos (+ pos 1))
	 )
       data)
      output)
    )
  )

(define make-extern-defs
  (lambda (type)
    (let* ((name   (assoc-ref type "name"))
	   )
      (string
       (tab-pad-string (string "extern void glibtop_get_" name) 6)
       "__P(glibtop *, glibtop_" name " *);\n\n"
       "#ifdef HAVE_GUILE\n\n"
       "/* You need to link with -lgtop_guile to get this stuff here. */\n\n"
       (tab-pad-string (string "extern SCM glibtop_get_" name) 6)
       "__P(void);\n\n"
       "#endif /* HAVE_GUILE */\n\n"
       "#ifdef GLIBTOP_GUILE_NAMES\n\n"
       "/* You need to link with -lgtop_guile_names to get this stuff here. */\n\n"
       (tab-pad-string (string "extern SCM glibtop_guile_names_" name) 6)
       "__P(void);\n"
       (tab-pad-string (string "extern SCM glibtop_guile_labels_" name) 6)
       "__P(void);\n"
       (tab-pad-string (string "extern SCM glibtop_guile_descriptions_" name) 6)
       "__P(void);\n\n"
       "#endif /* GLIBTOP_GUILE_NAMES */\n\n"
       "#ifdef GLIBTOP_NAMES\n\n"
       "/* You need to link with -lgtop_names to get this stuff here. */\n\n"
       (tab-pad-string (string "extern const char *glibtop_names_" name) 6)
       "[];\n"
       (tab-pad-string (string "extern const char *glibtop_labels_" name) 6)
       "[];\n"
       (tab-pad-string (string "extern const char *glibtop_descriptions_" name) 6)
       "[];\n\n"
       "#endif /* GLIBTOP_NAMES */\n\n")
      )
    )
  )

(begin
  (display (make-field-constants cpu-type)) (newline)
  (display (make-struct cpu-type)) (newline)
  (display (make-extern-defs cpu-type)) (newline)
  )

