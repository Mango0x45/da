# DA — Simple & Stupid Dynamic Array

DA is a simple and easy-to-use single-header library for a dynamic array.
It’s not meant to be the worlds most efficent or optimal implementation;
it’s just stupid and simple.  This means that it is perfect for 99% of
your dynamic array needs.

The library is documented in full in `da.h`, and you can easily grab it
and include it in your project with the following command:

```sh
wget 'https://git.sr.ht/~mango/da/blob/master/da.h'
```

Do note that the `da_foreach()` macro requires the `auto` keyword from C23.  If
you want to use an older standard of C, you can write the macro yourself to take
a type argument.
