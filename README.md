# uefi_var_edit
Modify UEFI variables.

Provide -lz option to the compiler in order to compile successfully.

## Example:

`dmptore -guid A04A27f4-DF00-4D42-B552-39511302113d -s dump.txt`

Modify the data with `uefi_var_edit dump.txt`

`dmptore -guid A04A27f4-DF00-4D42-B552-39511302113d -l out.txt`
