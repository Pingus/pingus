#!/usr/bin/perl -w

# print @ARGS;

$directory = $ARGV[0];
$command = "find $directory -name \"*.png\" | grep -v \".xvpics\" |";
open (IN, $command);
$oldsection = "---";
$firstsection = 1;
while ($line = <IN>) {
    $line =~ s/\n//;

    $name = $line;
    $name =~ s/.*\/(.*)\....$/$1/;

    $section = $line;
    $section =~ s/$directory\/(.*)\/.*\..../$1/;

    if ($section ne $oldsection) {
	if ($firstsection eq 0) {
	    print "}\n\n";
	}
	$firstsection = 0;
	print "section $section {\n";
    }
    
    open (IDENTIFY, "identify -verbose $line | grep -e Geometry |");
    $geometry = <IDENTIFY>;
    $geometry =~ s/\n//;
    $width = $geometry;
    $height = $geometry;

    $width =~ s/  Geometry: ([0-9]+)x([0-9]+)$/$1/;
    $height =~ s/  Geometry: ([0-9]+)x([0-9]+)$/$2/;

    print "	$name = $line (type=surface, x=0, y=0, width=$width, height=$height);\n";

    $oldsection = $section;
}

print "}\n\n";

# EOF #
