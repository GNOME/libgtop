;; $Id$

(use-modules (libgtop names))

(load "features.scm")

(define sysdeps-list '())

(define type-names '("void"
		     "int64_t" "u_int64_t" "double"
		     "int" "char" "const char *"))

(define make-sysdeps-list
  (lambda ()
    (letrec ((names (cdr glibtop-names-sysdeps))
	     (labels (cdr glibtop-labels-sysdeps))
	     (descriptions (cdr glibtop-descriptions-sysdeps))
	     )
      (for-each (lambda (feature)
		  (let* ((label (car labels))
			 (description (car descriptions))
			 (list '())
			 )
		    (set! labels (cdr labels))
		    (set! descriptions (cdr descriptions))
		    (set! list (assoc-set! list 'name feature))
		    (set! list (assoc-set! list 'label label))
		    (set! list (assoc-set! list 'description description))
		    (set! sysdeps-list (assoc-set! sysdeps-list feature list))
		    )
		  )
		names)
      )
    )
  )

(define make-function-reference
  (lambda (feature)
    (let* ((feature-name (car (assoc-ref libgtop-feature-names feature)))
	   (names (eval-string (string "glibtop-names-" feature-name)))
	   (types (eval-string (string "glibtop-types-" feature-name)))
	   (labels (eval-string (string "glibtop-labels-" feature-name)))
	   (sysdeps (assoc-ref sysdeps-list feature))
	   (retval (car (car (assoc-ref libgtop-features feature))))
	   (name (assoc-ref sysdeps 'name))
	   (label (assoc-ref sysdeps 'label))
	   (description (assoc-ref sysdeps 'description))
	   (descriptions (eval-string (string "glibtop-descriptions-" feature-name)))
	   (feature_nounder (car (assoc-ref libgtop-feature-names feature)))
	   (decl-list '()) (field-list '())

	   (synopsis-start-string
	    (string "<para>Automatically generated function synopsis:\n\n"))

	   (section-string
	    (string "<sect1>\n"
		    "<title><function>glibtop_get_" feature "()</function> - "
		    label "</title>\n\n")
	    )

	   (description-string
	    (string "<para>\nAutomatically generated description:\n\n"
		    "<blockquote>\n<literallayout>\n"
		    description "\n"
		    "</literallayout>\n</blockquote>\n\n")
	    )
	   
	   (definition-start-string
	     (string "<para>Automatically generated declaration of "
		     "<structname>_glibtop_" feature "</structname>:\n\n")
	     )

	   (param-description-start-string
	    (string "<para>Automatically generated descriptions of "
		    "<structname>_glibtop_" feature "</structname>:\n\n")
	    )

	   (funcsynopsisinfo-string
	    (string "<funcsynopsisinfo>\n"
		    "#include &lt;glibtop.h&gt;\n"
		    "#include &lt;glibtop/" feature ".h&gt;"
		    "</funcsynopsisinfo>")
	    )

	   (funcdef-string
	    (string "<funcdef>" retval " "
		    "<function>glibtop_get_" feature "_l</function>"
		    "</funcdef>\n")
	    )

	   (paramdef-string
	    (lambda ()
	      (let ((start (string "<paramdef>glibtop *<parameter>"
				   "server</parameter>, glibtop_"
				   feature " *<parameter>" feature
				   "</parameter>")
			   )
		    (param_string (string))
		    (end (string "\n</paramdef>"))
		    (param_lists
		     (cdr (car (assoc-ref libgtop-features feature))))
		    )
		(for-each
		 (lambda (x)
		   (let ((type (car x))
			 (params (cdr x))
			 )
		     (for-each
		      (lambda (param)
			(set! param_string (string param_string
						   ", " type
						   " <parameter>"
						   param
						   "</parameter>"
						   )
			      )
			)
		      params)
		     )
		   )
		 param_lists)
		(string start param_string end))
	      )
	    )

	   (funcdef-noserver-string
	    (string "<funcdef>" retval " "
		    "<function>glibtop_get_" feature "</function>"
		    "</funcdef>\n")
	    )

	   (paramdef-noserver-string
	    (string "<paramdef>glibtop_" feature " *"
		    "<parameter>" feature "</parameter>\n"
		    "</paramdef>")
	    )

	   (field-name-constant
	    (lambda (name field)
	      (string "GLIBTOP_"
		      (string-upcase! (string name))
		      "_"
		      (string-upcase! (string field)))
	      )
	    )

	   (make-struct-name
	    (lambda (type)
	      (string "glibtop_" (assoc-ref type "name"))
	      )
	    )
	   
	   (tab-pad-string
	    (lambda (string tabs)
	      (let* ((slength (string-length string))
		     (tlength (* tabs 8)))
		(string-append string
			       (make-string 
				(if (> tlength slength)
				    (- tlength slength)
				    1
				    )
				#\space))
		)
	      )
	    )

	   (make-field-list
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
					    (tab-pad-string
					     (field-name-constant name (car x)) 4)
					    " */"))
			   (field (tab-pad-string
				   (string-append (string (car x)) sep) 2))
			   )
		      (set! pos (+ pos 1))
		      (string-append start field comment "\n")
		      )
		    )
		  fields)
		 )
	       )
	     )

	   (init-field-list
	    (lambda ()
	      (let* ((old-type #f)
		     (type-list types)
		     (new-type #f)
		     (fields (list))
		     )
		(for-each
		 (lambda (x)
		   (set! new-type (car type-list))
		   (set! type-list (cdr type-list))
		   (set! new-type (list-ref type-names new-type))
		   (if (equal? old-type new-type)
		       (set-car! fields
				 (append (list new-type)
					 (cdr (car fields))
					 (list (list x))
					 )
				 )
		       (if (equal? old-type #f)
			   (set! fields (list (list new-type (list x))))
			   (set! fields (append (list (list new-type (list x))) fields))
			   )
		       )
		   (set! old-type new-type)
		   )
		 names)
		fields)
	      )
	    )

	   (make-struct-body
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
	   
	   (make-struct
	    (lambda (type)
	      (let* ((name   (assoc-ref type "name"))
		     (data   (assoc-ref type "fields"))
		     (output (string-append
			      (tab-pad-string
			       (string "typedef struct _glibtop_" name)
			       5)
			      (string "glibtop_" name ";\n\n"
				      "struct _glibtop_" name "\n{\n\t"
				      "u_int64_t\tflags;\n")
			      )
			     )
		     )
		(string-append output (make-struct-body type) "};\n")
		)
	      )
	    )

	   (make-param-description
	    (lambda ()
	      (let* ((label-list labels)
		     (description-list descriptions)
		     (output (string))
		     )
		(for-each
		 (lambda (name)
		   (let* ((label (car label-list))
			  (description (car description-list))
			  )
		     (set! label-list (cdr label-list))
		     (set! description-list (cdr description-list))
		     (set! output
			   (string-append output 
					  (string "<varlistentry>\n"
						  "<term><parameter>"
						  name
						  "</parameter></term>\n\n"
						  "<listitem>\n<para>\n"
						  description "\n\n")
					  )
			   )
		     )
		   )
		 names)
		output)
	      )
	    )
	   
	   )
      
      (set! decl-list (assoc-set! decl-list "name" name))
      (set! decl-list (assoc-set! decl-list "label" label))
      (set! decl-list (assoc-set! decl-list "fields" (init-field-list)))

      (string-append section-string
		     description-string
		     synopsis-start-string
		     (string "<blockquote>\n<funcsynopsis>\n")
		     funcsynopsisinfo-string
		     funcdef-noserver-string
		     paramdef-noserver-string
		     (string "</funcsynopsis>\n")
		     (string "<funcsynopsis>\n")
		     funcdef-string
		     (paramdef-string)
		     (string "</funcsynopsis>\n</blockquote>\n")
		     definition-start-string
		     (string "<blockquote>\n<literallayout>\n")
		     (make-struct decl-list)
		     (string "</literallayout>\n</blockquote>\n")
		     param-description-start-string
		     "<variablelist>\n\n"
		     (make-param-description)
		     "</variablelist>\n\n"
		     "&include-" feature_nounder ".sgml;\n\n"
		     )
      )
    )
  )

;; <sect2 id="glibtop-get-cpu">
;; <title><function>glibtop_get_cpu()</function> - get CPU usage</title>
;; 
;; <funcsynopsis>
;; <funcsynopsisinfo>
;; #include &lt;glibtop.h>
;; #include &lt;glibtop/cpu.h></funcsynopsisinfo>
;; <funcdef>void <function>glibtop_get_cpu</function></funcdef>
;; <paramdef>glibtop *<parameter>server</parameter>, glibtop_cpu *<parameter>cpu_usage</parameter>
;; </paramdef></funcsynopsis>

(begin
  (make-sysdeps-list)
  
  (for-each (lambda (x)
	      (display (make-function-reference x))
	      )
	    (cdr glibtop-names-sysdeps))
  )
