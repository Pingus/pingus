;; Pingus controller for R36S / ArkOS handhelds (PortMaster).
;; Left stick moves the software cursor; face buttons act like mouse/keys.
;; Joystick device 0 is the built-in GO-Super (or first pad). Keyboard kept
;; as a fallback when a dock/Bluetooth keyboard is attached.
(pingus-controller
 (controls
  (standard-keyboard
   (sdl:keyboard))

  (standard-pointer
   (sdl:mouse-pointer)
   (core:axis-pointer
    (x-axis (sdl:joystick-axis (device 0) (axis 0)))
    (y-axis (sdl:joystick-axis (device 0) (axis 1)))))

  (standard-scroller
   (core:button-scroller
    (up    (sdl:keyboard-button (key "Up")))
    (down  (sdl:keyboard-button (key "Down")))
    (left  (sdl:keyboard-button (key "Left")))
    (right (sdl:keyboard-button (key "Right"))))
   (core:axis-scroller
    (x-axis (sdl:joystick-axis (device 0) (axis 2)))
    (y-axis (sdl:joystick-axis (device 0) (axis 3)))))

  ;; A / B (Xbox layout on many ArkOS mappings)
  (primary-button
   (sdl:mouse-button (button 1))
   (sdl:joystick-button (device 0) (button 0)))

  (secondary-button
   (sdl:mouse-button (button 2))
   (sdl:mouse-button (button 3))
   (sdl:joystick-button (device 0) (button 1)))

  ;; Start / Select
  (pause-button
   (sdl:keyboard-button (key "P"))
   (sdl:joystick-button (device 0) (button 7)))

  (fast-forward-button
   (sdl:keyboard-button (key "F"))
   (sdl:keyboard-button (key "Space"))
   (sdl:joystick-button (device 0) (button 5)))

  (single-step-button
   (sdl:keyboard-button (key "S"))
   (sdl:joystick-button (device 0) (button 4)))

  (armageddon-button
   (sdl:keyboard-button (key "A"))
   (sdl:joystick-button (device 0) (button 3)))

  (escape-button
   (sdl:keyboard-button (key "Escape"))
   (sdl:keyboard-button (key "AC Back"))
   (sdl:joystick-button (device 0) (button 6)))

  ;; X / Y or shoulder for action cycle
  (action-up-button
   (sdl:mouse-button (button 4))
   (sdl:joystick-button (device 0) (button 2)))

  (action-down-button
   (sdl:keyboard-button (key "Tab"))
   (sdl:mouse-button (button 5))
   (sdl:joystick-button (device 0) (button 3)))

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
  ))

;; EOF ;;
