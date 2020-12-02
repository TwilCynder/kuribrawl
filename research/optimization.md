# Rendering
## Benchmarks

Action|ms|ns|
-|-|-|
ClipSprite | 0.00007 | 0.07
DisplayTransparentSprite (screen) |0.00231|2.31|
Drawing an image on Screen | 0.01790 | 17.9
Drawing an image on Image | 0.00770 | 7.7
Drawing an image on Sprite | 0.00770 | 7.7
StartDrawing on a sprite | 0.04130 | 41.3
StartDrawing on an Image | 0.05460 | 54.6
StartDrawing on Screen | 0.75580 | 755.8
Drawing transparent circle on Sprite | 0.09240 | 92.4
Drawing transparent circle on Image | 0.09260 | 92.6
Drawing transparent circle on Screen | 0.01120 | 11.2

## Conclusion

2DDrawing functions are generally faster on screen (around 8-9 times). However, startDrawing will be 18-14 times slower on Screen than on a Sprite/Image, almost reaching the whole milisecond. As such, direct drawing on Screen should not be considered an option.

Drawing Image is 2-3 times faster on Image/Sprite than on screen, and drawing a sprite (to screen) is the fastest function (2ns)

ClipSprite takes virtually no time (less than a nanosecond), having to clip a sprite before drawing it is not a problem.

/!\ DISPLAYSPRITE CAN'T BE USED DURING DIRECT DRAWING

Since sprites can be displayed to screen extremely fast, and StartDrawing is extremely slow on screen, it seems like the best method to draw anything would be to do it on a sprite and then displaying the sprite. However, if we are 2DDrawing a lots of things, the fact it is 8 times faster on Screen than on Sprite might make it faster to 2ddraw directly on screen ?

## Experiments on the game code
Changing the Image that was used to render shields and hitboxes to a sprite didn't go well : render time went from 0-1ms to 9-10. (approx 5ms per renderFighter()) It would seem that for some reason, direct drawing on sprites is a generally bad idea.

## Summary of the different drawing methods

Name|Capabilities|Speed
-|-|-
DisplaySprite|On screen directly|Fastest by far
