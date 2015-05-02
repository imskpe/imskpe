FROM		ubuntu:14.04
MAINTAINER	Andreas Madsack <andreas@madflex.de>

RUN		apt-get update -qq

RUN		apt-get install -qqy build-essential
RUN		apt-get install -qqy libgtk2.0-dev
RUN		apt-get install -qqy git
RUN		apt-get install -qqy flex

RUN		git clone https://github.com/imskpe/imskpe.git

WORKDIR		/imskpe/imskpe
RUN		./configure LIBS="-lm"
RUN		make
