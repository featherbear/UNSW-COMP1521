cat isi.s tests/m3.s > exe.s
~cs1521/bin/spim -file exe.s | sed -e 1d
