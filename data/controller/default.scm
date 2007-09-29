(pingus-controller
 (standard-keyboard
  (sdl:keyboard))

  (standard-pointer
    (sdl:mouse-pointer))

  (standard-scroller
   (evdev:scroller (device "/dev/input/spacenavigator") (x 0) (y 1))
   (core:button-scroller
    (up    (sdl:keyboard-button (key "up")))
    (down  (sdl:keyboard-button (key "down")))
    (left  (sdl:keyboard-button (key "left")))
    (right (sdl:keyboard-button (key "right")))))

  (primary-button
    (sdl:mouse-button (button 1))
    (evdev:button (device "/dev/input/spacenavigator") (button 256)))

  (secondary-button
    (sdl:mouse-button (button 3))
    (evdev:button (device "/dev/input/spacenavigator") (button 257)))

  (pause-button
   (sdl:keyboard-button (key "p")))

  (fast-forward-button
   (sdl:keyboard-button (key "f"))
   (sdl:keyboard-button (key "space")))

  (armageddon-button
    (sdl:keyboard-button (key "a")))

  (escape-button
    (sdl:keyboard-button (key "escape")))

;;   (scroller
;;     (axis-scroller
;;       (speed 1000)
;;       (button-axis
;;         (angle 90)
;;         (key-button
;;           (key "down"))
;;         (key-button
;;           (key "up")))
;;       (button-axis
;;         (angle 0)
;;         (key-button
;;           (key "right"))
;;         (key-button
;;           (key "left")))))

;;   (action-buttons
;;     (action-button (key-button (key 1)))
;;     (action-button (key-button (key 2)))
;;     (action-button (key-button (key 3)))
;;     (action-button (key-button (key 4)))
;;     (action-button (key-button (key 5)))
;;     (action-button (key-button (key 6)))
;;     (action-button (key-button (key 7)))
;;     (action-button (key-button (key 8)))
;;     (action-button (key-button (key 9)))
;;     (action-button (key-button (key 0))))

;;   (action-up
;;     (multiple-button
;;       (key-button
;;         (key "prior"))
;;       (mouse-button
;;         (button 5))))

;;   (action-down
;;     (multiple-button
;;       (key-button
;;         (key "next"))
;;       (mouse-button
;;         (button 4))))
  )

;; EOF ;;
