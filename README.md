# catmd
catmd is a program that will output the given md file to stdout and prettify it,

try it on this README and see how it works

---

# usage

`catmd [file]`

**note**: options will be passed to `$readCmd` (`cat` by default), this is unintended and might break, however it is useful to know

# installation

```sh
git clone https://github.com/euro20179/catmd
cd catmd
g++ catmd.cpp -o catmd
chmod +x catmd
sudo cp catmd /usr/local/bin
```
