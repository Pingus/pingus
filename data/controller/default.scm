(pingus-controller
  (standard-pointer
    (mouse-pointer ))

  (primary-button
    (mouse-button
      (button 1)))

  (secondary-button
    (mouse-button
      (button 3)))

  (pause-button
    (key-button
      (key "p")))

  (fast-forward-button
    (multiple-button
      (key-button
        (key "f"))
      (key-button
        (key "space"))))

  (armageddon-button
    (key-button
      (key "a")))

  (escape-button
    (key-button
      (key "escape")))

  (scroller
    (axis-scroller
      (speed 1000)
      (button-axis
        (angle 90)
        (key-button
          (key "down"))
        (key-button
          (key "up")))
      (button-axis
        (angle 0)
        (key-button
          (key "right"))
        (key-button
          (key "left")))))

  (action-buttons
    (action-button (key-button (key 1)))
    (action-button (key-button (key 2)))
    (action-button (key-button (key 3)))
    (action-button (key-button (key 4)))
    (action-button (key-button (key 5)))
    (action-button (key-button (key 6)))
    (action-button (key-button (key 7)))
    (action-button (key-button (key 8)))
    (action-button (key-button (key 9)))
    (action-button (key-button (key 0))))

  (action-up
    (multiple-button
      (key-button
        (key "prior"))
      (mouse-button
        (button 5))))

  (action-down
    (multiple-button
      (key-button
        (key "next"))
      (mouse-button
        (button 4)))))
