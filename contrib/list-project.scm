(use-modules (srfi srfi-13))

(define (file->list filename)
  (let ((port (open-input-file filename))
        (lst  '()))
    (let loop ((line (read-line port)))
      (cond ((not (eof-object? line))
             (set! lst (cons line lst))
             (loop (read-line port)))))
    (close port)
    (reverse lst)))

(define (filter pred? objects)
  (let loop ((objs objects)
	     (result '()))
    (cond ((null? objs) (reverse! result))
	  ((pred? (car objs)) (loop (cdr objs) (cons (car objs) result)))
	  (else (loop (cdr objs) result)))))

(define (strip-comment line)
  (let ((comment-start (string-index line #\#)))
    (if comment-start
        (substring line 0 comment-start)
        line)))

(define (line-empty? line)
  (let ((l (string-length line))
        (line-empty #t))
    (do ((i 0 (1+ i)))
        ((or (not (< i l))
             (not line-empty)))
      (if (not (char-whitespace? (string-ref line i)))
          (set! line-empty #f)))
    line-empty))

(define (last-char line)
  (string-ref line (- (string-length line) 1)))

(define (merge-and-clean current-line lines)
  (cond ((null? lines) ;; end of file
         (if (and current-line (not (line-empty? current-line)))
             (cons current-line '())
             '()))
        (else ;; lines to parse still there
         (let ((next-line (car lines)))
           (cond ((line-empty? next-line)
                  (if current-line
                      (cons current-line (merge-and-clean #f (cdr lines)))
                      (merge-and-clean #f (cdr lines))))
                 (else
                  (cond ((char=? #\\ (last-char next-line))
                         (if current-line
                             (merge-and-clean (string-append current-line
                                                             (substring next-line 
                                                                        0
                                                                        (- (string-length next-line) 1)))
                                              (cdr lines))
                             (merge-and-clean (substring next-line 0 (- (string-length next-line) 1))
                                              (cdr lines))))
                        (else
                         (if current-line
                             (cons (string-append current-line next-line)
                                   (merge-and-clean #f (cdr lines)))
                             (cons next-line (merge-and-clean #f (cdr lines)))
                             )))))))))

(define (parse-Makefile.am-line line)
  (cond ((or (< (length line) 3)
             (not (string=? (cadr line) "=")))
         '(unknown line))
        (else
         (list 'variable (car line) (cddr line)))))

(define (parse-Makefile.am content)
  (cond ((null? content)
         '())
        (else
         (let ((line (car content)))
           (cons (parse-Makefile.am-line (string-tokenize line))
                 (parse-Makefile.am (cdr content))
                 )))))

(define (read-Makefile.am filename)
  (let* ((plain-content    (file->list filename))
         (filtered-content (merge-and-clean #f (map strip-comment plain-content))))
    (parse-Makefile.am filtered-content)))

(define (dump-source-files filename)
  (let ((parse-tree (read-Makefile.am filename))
        (dname      (dirname filename)))
    (dump-source-files-tree dname parse-tree)))

(define (suffix? suffix line)
  (cond ((<= (string-length suffix)
             (string-length line))
         (string=? (substring line (- (string-length line) (string-length suffix)))
                   suffix))
        (else
         #f)))

(define (dump-source-files-tree dname tree)
  (cond ((null? tree)
         '())
        (else
         (let ((type (caar tree)))
           (case type
             ((variable)
              (cond ((suffix? "SOURCES" (cadar tree))
                     (append (map (lambda (filename)
                                    (string-append dname "/" filename))
                                  (caddar tree))
                             (dump-source-files-tree dname (cdr tree))))
                    (else
                     ;;(display (car tree))(newline)
                     (dump-source-files-tree dname (cdr tree)))))
             (else
              (dump-source-files-tree dname (cdr tree))))))))


(for-each (lambda (file)
            (format #t "~A\n" file))
          (sort (append (dump-source-files "src/colliders/Makefile.am")
                        (dump-source-files "src/particles/Makefile.am")
                        (dump-source-files "src/input/axes/Makefile.am")
                        (dump-source-files "src/input/pointers/Makefile.am")
                        (dump-source-files "src/input/Makefile.am")
                        (dump-source-files "src/input/scrollers/Makefile.am")
                        (dump-source-files "src/input/buttons/Makefile.am")
                        (dump-source-files "src/sound/Makefile.am")
                        (dump-source-files "src/worldobjs/Makefile.am")
                        (dump-source-files "src/worldobjs/entrances/Makefile.am")
                        (dump-source-files "src/editor/Makefile.am")
                        (dump-source-files "src/Makefile.am")
                        (dump-source-files "src/worldmap/Makefile.am")
                        (dump-source-files "src/movers/Makefile.am")
                        (dump-source-files "src/actions/Makefile.am")
                        (dump-source-files "src/worldobjsdata/Makefile.am")
                        (dump-source-files "src/editorobjs/Makefile.am")
                        (dump-source-files "src/caimagemanipulation/Makefile.am"))
                string<=?))

;; EOF ;;