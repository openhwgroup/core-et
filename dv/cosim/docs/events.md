## CoSim Events

**Note:** This part of CoSim is still in active development.

CoSim events are described in an event file (.evt).
This file is parsed to generate C++ and Verilog-compatible header files (see [tools/events.py](../tools/events.py))
For example, from `cosim.evt` we will generate `cosim_evt.vh` for the Verilog side, and `cosim_evt.cc` and `cosim_evt.h` for the C++ side.
For more information on how to run the generator script see `tools/events.py --help`.

### Event Definition

Each event lives in a namespace, based on the module name.

An event is defined as follows:

    event SomeEvent {
        foo: u32, 
        bar: i64,
    }


In the Verilog side, this event will be rendered as unpacked struct.
Additionally, we define a C-API function (`trigger_SomeEvent(input SomeEvent)`) which is the main way to trigger events.

The C++ counterpart takes a `const` pointer to an event struct with the same layout (i.e.: unpacked struct).
Furthermore, each event has a unique ID, and helper functions (e.g.: `event::describe()`).

#### Data types

The following table shows the supported data types and the corresponding C++/Verilog types.

|Type  | C/C++     | SystemVerilog     |
|------|-----------|-------------------|
| u8   | uint8_t   | byte unsigned     |
| u16  | uint16_t  | shortint unsigned |
| u32  | uint32_t  | int unsigned      |
| u64  | uint64_t  | longint unsigned  |
| i8   | int8_t    | byte              |
| i16  | int16_t   | shortint          |
| i32  | int32_t   | int               |
| i64  | int64_t   | longint           |
| f32  | float32_t | shortreal         |
| f64  | float64_t | real              |
| bool | bool      | bit               |

All of these types support arrays (e.g.: `u8[10]`). These arrays will be unpacked as well.

Constants are supported as well by using the `const` keyword.
These will be evaluated as the event file is parsed.

#### Other notes

There is an example event file in the tools directory ([tools/example.evt](../tools/example.evt)).
You can also find a vim syntax file ([tools/event.vim](../tools/event.vim)).


### Possible alternatives

There are more conventional serialization libraries available ([capnproto], [flatbuffers], or [protobuf]).
One of the main differences with these is that the binary layout of the struct matches the definition.
This means we can use these structs from Verilog as input for our DPIs.

[capnproto](https://capnproto.org/index.html)
[flatbuffers](https://google.github.io/flatbuffers/)
[protobuf](https://developers.google.com/protocol-buffers)

