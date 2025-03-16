run :
	@docker-compose -f config/compose.yml -p c-http up --build -d

stop :
	@docker-compose -f config/compose.yml -p c-http down --remove-orphans
