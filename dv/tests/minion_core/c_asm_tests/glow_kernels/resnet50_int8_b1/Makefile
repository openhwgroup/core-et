all:
	${REPOROOT}/swx/glow/build.sh --fastRun 
	${REPOROOT}/nn/scripts/run_model_glow.pl -skipBuild 1 -model resnet50 -batch 1 -quantized 1
	cp resnet50*/* .

clean:
	rm -rf resnet50* *.html *.o *.txt bootrom* tensor* out* *.cc *.mem *.h *.S net_test* gpu_0_softmax *.log

