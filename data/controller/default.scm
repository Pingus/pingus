(pingus-controller
 (standard-keyboard
  (sdl:keyboard))

  (standard-pointer
    (sdl:mouse-pointer))

  (standard-scroller
   (core:button-scroller
    (up    (sdl:keyboard-button (key "up")))
    (down  (sdl:keyboard-button (key "down")))
    (left  (sdl:keyboard-button (key "left")))
    (right (sdl:keyboard-button (key "right"))))

   (core:button-scroller
    (up    (sdl:keyboard-button (key "w")))
    (down  (sdl:keyboard-button (key "s")))
    (left  (sdl:keyboard-button (key "a")))
    (right (sdl:keyboard-button (key "d")))))

  (primary-button
    (sdl:mouse-button (button 1)))

  (secondary-button
    (sdl:mouse-button (button 2))
    (sdl:mouse-button (button 3)))

  (pause-button
   (sdl:keyboard-button (key "r"))
   (sdl:keyboard-button (key "p")))

  (fast-forward-button
   (sdl:keyboard-button (key "space")))

  (single-step-button
   (sdl:keyboard-button (key "f")))

  (armageddon-button
   (sdl:keyboard-button (key "backspace")))

  (escape-button
   (sdl:keyboard-button (key "escape")))

  (action-up-button
   (sdl:keyboard-button (key "q"))
   (sdl:keyboard-button (key "page up"))
   (sdl:mouse-button (button 4)))
  
  (action-down-button
   (sdl:keyboard-button (key "e"))
   (sdl:keyboard-button (key "tab"))
   (sdl:keyboard-button (key "page down"))
   (sdl:mouse-button (button 5)))

  (action-1-button  (sdl:keyboard-button (key "1")))
  (action-2-button  (sdl:keyboard-button (key "2")))
  (action-3-button  (sdl:keyboard-button (key "3")))
  (action-4-button  (sdl:keyboard-button (key "4")))
  (action-5-button  (sdl:keyboard-button (key "5")))
  (action-6-button  (sdl:keyboard-button (key "6")))
  (action-7-button  (sdl:keyboard-button (key "7")))
  (action-8-button  (sdl:keyboard-button (key "8")))
  (action-9-button  (sdl:keyboard-button (key "9")))
  (action-10-button (sdl:keyboard-button (key "0")))
  )

;; EOF ;;
