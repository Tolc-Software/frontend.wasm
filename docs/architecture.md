# Architecture #

### Builders are smart, they are building dumb objects ###

You should see a strong connection between the `Builders` and the `Embind::Proxy` objects they are building. You should consider the objects as data aggregations, with builders making decisions on how they are stored the best. **The `Embind::Proxy` classes should not make decisions.**

## Good to know ##

### Emscripten internals: --bind flag will soon be renamed to -lembind ###

This better reflects that it just links with the library `embind`.

### The test stage contains `embind` examples ###

The example `embind` module in `tests/testStage/src/myModule.cpp` contains a buildable module with example code.

Whenever there are discoveries of new ways to do things in `embind`, they should be added there. This will not affect any tests.

