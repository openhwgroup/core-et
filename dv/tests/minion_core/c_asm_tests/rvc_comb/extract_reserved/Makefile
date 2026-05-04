all: reserved.json

expander=$(RTLROOT)/shire/minion/frontend/frontend_rvc_expander.v
VCS_OPTS=-sverilog -full64 +incdir+$(RTLROOT)/inc $(TRACE)

reserved.json: test.log parse.py
	./parse.py

test.log: simv
	./simv > test.log

simv: test.v $(expander)
	vcs $(VCS_OPTS) test.v $(expander)

