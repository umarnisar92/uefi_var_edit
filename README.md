# uefi_var_edit
Modify UEFI variables.

Provide -lz option to the compiler in order to compile successfully.

## Example:

`dmptore -guid A04A27f4-DF00-4D42-B552-39511302113d -s dump.txt`

Modify the data with `.\uefi_var_edit dump.txt`

`dmptore -guid A04A27f4-DF00-4D42-B552-39511302113d -l out.txt`

### An example of data modification:

Change Data byte (offset 0x38) from 0x07 to 0x08.

```
4c4
< 00000030: 1302 113d 0300 0000 074f 1b96 9818 0000  ...=.....O......
---
> 00000030: 1302 113d 0300 0000 08de 0629 0818 0000  ...=.......)....
```
