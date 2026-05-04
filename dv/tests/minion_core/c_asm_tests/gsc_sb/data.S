.data
.global buffer
.align 8
buffer: 
.incbin "data.raw"            // generated with dd if=/dev/urandom of=data.raw bs=1M count=1
