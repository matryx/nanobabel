if [ "$(docker ps -aq -f name=nanome-nanobabel)" != "" ]; then
    # cleanup
    echo "removing exited container"
    docker rm -f nanome-nanobabel
fi

docker run -d \
--name nanome-nanobabel \
--restart unless-stopped \
-e ARGS="$*" \
nanome-nanobabel