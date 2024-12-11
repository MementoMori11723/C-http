FROM gcc:latest
WORKDIR /usr/src/app
COPY app.c .
copy *.html .
RUN gcc -o app app.c
EXPOSE 8000
CMD ["./app"]
