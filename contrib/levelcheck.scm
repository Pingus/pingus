#!/usr/bin/guile \
--debug -e main -s
!#

(use-modules (ice-9 format))

(define *pingu-level-spec*
  '(pingus-level (hashmap
                  (children
                   (version (int))
                   (head
                    (hashmap
                     (children
                      (levelname        (string))
                      (description      (string))
                      (author           (string))
                      (number-of-pingus (int))
                      (number-to-save   (int))
                      (time             (int))
                      (difficulty       (int))
                      (playable         (int))
                      (comment          (string))
                      (music            (string))
                      ;; (actions          (string))
                      )))
                   (objects 
                    (sequence
                      (children
                       )))
                   ))))

(define (element-position el)
  (if (pair? el)
      (format #f "~a:~a"
              (source-property el 'filename)
              (source-property el 'line))
      (format #f "unknown: ~a" el)))

(define (verify-hashmap spec tree)
  (cond ((list? tree)
         (let loop ((children (assoc-ref (cdr spec) 'children))
                    (subtree  tree))
           (if (not (null? children))
               (cond ((null? subtree)
                      (format #t "~a: Error: Missing elements in:\n~a\nExpected:\n~a\n"
                              (element-position tree)
                              tree 
                              children))
                     (else
                      (verify-element (car children) (car subtree))
                      
                      (loop (cdr children) (cdr subtree)))))))
        (else
         (format #t "~a: Error:  Expected hashmap, got " (element-position tree))
         #f)))

(define (verify-sequenc spec tree)
  #t)

(define (verify-element spec tree)
  (let ((tag      (car  spec))
        (type     (cadr spec)))

    (cond ((not (list? tree))
           (format #t "Error: Expected element, got ~a\n" tree)
           #f)
          
          ((equal? tag (car tree))
           (format #t "Element '~a' ok\n" tag)
           (cond ((equal? (car type) 'string)
                  (if (string? (cadr tree))
                      #t
                      (format #t "~a: Error: Expected string element got '~s'\n"
                              (element-position tree) (cadr tree))))
                 ((equal? (car type) 'int)
                  (if (integer? (cadr tree))
                      #t
                      (format #t "~a: Error: Expected integer element got '~s'\n"
                              (element-position tree) (cadr tree))))
                 ((equal? (car type) 'real)
                  (if (real? (cadr tree))
                      #t
                      (format #t "~a: Error: Expected real element got '~s'\n"
                              (element-position tree) (cadr tree))))
                 ((equal? (car type) 'bool)
                  (if (boolean? (cadr tree))
                      #t
                      (format #t "~a: Error: Expected bool element got '~s'\n"
                              (element-position tree) (cadr tree))))
                 ((equal? (car type) 'hashmap)
                  (verify-hashmap type (cdr tree)))
                 ((equal? (car type) 'sequence)
                  #t (verify-sequenc type (cdr tree)))))
          (else
           (format #t "Error: Expected element '~a', but got '~a'\n" tag (car tree))
           #f))))

(define (read-file filename)
  (let* ((port (open-input-file filename))
         (ret  (read port)))
    (close-input-port port)
    ret))

(define (main args)
  (read-enable 'positions)

  (cond ((= (length args) 1)
         (format #t "Usage: ~a FILENAME...\n" (car args)))
        (else
         (for-each (lambda (filename)
                     (format #t "Filename: '~a'\n" filename)
                     (let ((content (read-file filename)))
                       (verify-element *pingu-level-spec* content)))
                   (cdr args)))))

; EOF ;;
