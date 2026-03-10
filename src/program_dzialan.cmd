#define SPEED 5
#define ROT_SPEED 60
#define PAUSE_TIME 1000

SetColor Wally 255 0 0
SetColor Ewa 0 0 255

Begin_Parallel_Actions

  Begin_Sequential_Actions
    Move Ewa SPEED 10
    Rotate Ewa OZ ROT_SPEED 90
    Move Ewa SPEED 5
    Rotate Ewa OZ ROT_SPEED 180
    SetColor Ewa 0 255 255
  End_Sequential_Actions

  Begin_Sequential_Actions
    Move Wally SPEED 5
    Rotate Wally OZ ROT_SPEED 90
    Move Wally SPEED 5
    Rotate Wally OZ ROT_SPEED 90
    Move Wally SPEED 5
    Rotate Wally OZ ROT_SPEED 90
    Move Wally SPEED 5
    Rotate Wally OZ ROT_SPEED 90
    SetColor Wally 255 255 0
  End_Sequential_Actions

  Begin_Sequential_Actions
     Rotate Wally.Korpus.Glowa OZ 30 360
  End_Sequential_Actions

End_Parallel_Actions

Pause PAUSE_TIME
SetColor Wally 0 255 0
SetColor Ewa 0 255 0