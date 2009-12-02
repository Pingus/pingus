#!/bin/sh

xsltproc pingusv1tov2.xsl "$1" | ./xml2sexpr.rb

# EOF #
