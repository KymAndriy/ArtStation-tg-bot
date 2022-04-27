FROM reo7sp/tgbot-cpp


# RUN apt-get -qq install curl

COPY build/ .
RUN cmake .
RUN make -j4
CMD ./tg_bot