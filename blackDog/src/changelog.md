#changelog
All noteable changes to this project will be document in this file.

## [Unreleased]
- Input multipal purpose.
- array for init a new input.

## [1.0.0] 2022-08-20
### Added
- Released the first version of the myButton library.
    - config
    + inputConfig.
    + inputSetDebounce.
    + inputSetClickTime.
    + inputSetPressTime.
    + inputSetLongPressTime.

    - Runtime
    + inputTick.
    
    - event
    + inputAttachClick.
    + inputAttachDoubleClick.
    + inputAttachMultiClick.
    + inputAttachPress.
    + inputAttachLongPress.
    + inputAttachDepress.
    * function pointer struct member 'attachClick' in struct 'myButton' will be removed soon.

     methodes
    + inputGetStatus.
    + inputGetClickCounter.
    + inputGetActiveTime.
    + inputGerDeactiveTime.

