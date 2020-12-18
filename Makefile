test-greenhouse-window-controller:
	@$(MAKE) -C greenhouse-window-controller/ cli
	@$(MAKE) -C greenhouse-window-controller/ deps
	@$(MAKE) -C greenhouse-window-controller/ venv
	@$(MAKE) -C greenhouse-window-controller/ compile
