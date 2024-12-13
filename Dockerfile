FROM gcc:latest as builder
WORKDIR /usr/src/app
COPY . .
RUN gcc -o app app.c
FROM alpine:latest
RUN apk add --no-cache libc6-compat
WORKDIR /usr/src/app
COPY --from=builder /usr/src/app /usr/src/app
EXPOSE 8000
CMD ["./app"]
