.pragma library

var LeaderPosition = {
    UpperLeft : 0,
    Top : 1,
    UpperRight: 2,
    Right: 3,
    LowerRight: 4,
    Bottom: 5,
    LowerLeft: 6,
    Left: 7,
    Automatic: 8
}

// Technically this is just an internal enum only
// User doesn't have to set this but don't know
// of a way to declare a QML-only enum.
var LeaderMoveDirection = {
    Right : 0,
    Left : 1,
    Up: 2,
    Down: 3
}
