run :
	@docker-compose -f compose.yml build
	@docker-compose -f compose.yml up -d

stop :
	@docker-compose -f compose.yml down --remove-orphans
