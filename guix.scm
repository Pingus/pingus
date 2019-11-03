;; Pingus - A free Lemmings clone
;; Copyright (C) 2019 Ingo Ruhnke <grumbel@gmail.com>
;;
;; This program is free software: you can redistribute it and/or modify
;; it under the terms of the GNU General Public License as published by
;; the Free Software Foundation, either version 3 of the License, or
;; (at your option) any later version.
;;
;; This program is distributed in the hope that it will be useful,
;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;; GNU General Public License for more details.
;;
;; You should have received a copy of the GNU General Public License
;; along with this program.  If not, see <http://www.gnu.org/licenses/>.

(use-modules (guix build-system gnu)
             (guix gexp)
             (guix git-download)
             (guix licenses)
             (guix packages)
             (gnu packages boost)
             (gnu packages game-development)
             (gnu packages gl)
             (gnu packages image)
             (gnu packages pkg-config)
             (gnu packages python-xyz)
             (gnu packages sdl))

(define %source-dir (dirname (current-filename)))

(define (source-predicate . dirs)
  (let ((preds (map (lambda (p)
                      (git-predicate (string-append %source-dir p)))
                    dirs)))
    (lambda (file stat)
      (let loop ((f (car preds))
                 (rest (cdr preds)))
        (if (f file stat)
            #t
            (if (not (nil? rest))
                (loop (car rest) (cdr rest))
                #f))))))

(define-public pingus
  (package
   (name "pingus")
   (version "0.7.6")
   (source (local-file %source-dir
                       #:recursive? #t
                       #:select? (source-predicate ""
                                                   "/external/logmich"
                                                   "/external/tinygettext")))
   (build-system gnu-build-system)
   (arguments
     '(#:make-flags (list (string-append "PREFIX=" %output))
       #:tests? #f                      ; no check target
       #:phases
       (modify-phases %standard-phases
         (delete 'configure)))) ; no configure script
   (native-inputs `(("pkg-config" ,pkg-config)
                    ("scons" ,scons)))
   (inputs `(("sdl" ,sdl)
             ("sdl-image" ,sdl-image)
             ("sdl-mixer" ,sdl-mixer)
             ("mesa" ,mesa)
             ("glu" ,glu)
             ("libpng" ,libpng)
             ("boost" ,boost)))
   (synopsis "Guide groups of penguins through various obstacles")
   (description "A 2d side scrolling puzzle game where you guide large
groups of little penguins through various obstacles and dangers using
their numerous talents.

A level editor is included to allow building your own levels and
numerous user contributed levels are included as well.

The game is heavily inspired by the classic Lemmings.")
   (home-page "https://pingus.seul.org")
   (license gpl3+)))

pingus

;; EOF ;;
