#!/usr/bin/perl -w
#
# json.pl
#
# This CSRCompiler PerlAPI script creates the json file.
#
# $Revision: 1.1 $
#
use strict;
use csrPerlAPI;
use Math::BigInt;

my $perlPackageName = getPerlPkgName();
my $perlApiObject = csrPerlAPI->new($perlPackageName);
my $topAddressmapNode = $perlApiObject->getNextTopAddressmapNode();

&printJsonFile($topAddressmapNode);


sub printJsonFile {
   my ($addressmapNode) = @_;
   my $name;
   my $filename;
   my $filehandle;
   my $entry;
   my $registerComma;

   $name = $addressmapNode->getIdentifier();
   $filename = $name . ".json";
   open($filehandle, ">$filename") || die "Could not open $filename\n";

   if ($addressmapNode->hasDescription()) {
      print $filehandle
         "\"description\":\"" . $addressmapNode->getDescription() . "\",\n";
   }

   print $filehandle "\"data\":{\n";
   print $filehandle &getIndent(1) . "\"Csr\":{";
   $registerComma = "";
   while ($entry = $addressmapNode->getNextDesignNode()) {
      if ($entry->isRegisterNode()) {
         print $filehandle $registerComma . "\n";
         &printJsonRegister($filehandle, 2, $addressmapNode, $entry);
         $registerComma = ",";
      }
   } 
   print $filehandle "\n";
   print $filehandle &getIndent(1) . "}\n";
   print $filehandle "}\n";
}


sub printJsonRegister {
   my ($filehandle, $indent, $addressmapNode, $entry) = @_;
   my $index;
   my $field;
   my @fields;
   my $fieldInfo;
   my $fieldComma;

   print $filehandle &getIndent($indent) .
      "\"" . $entry->getIdentifier() . "\":{\n";

   if ($entry->hasDescription()) {
      print $filehandle &getIndent($indent + 1) .
         "\"blurb\":\"" . $entry->getDescription() . "\",\n";
   }

   if ($entry->hasAttribute('category')) {
      print $filehandle &getIndent($indent + 1) .
         "\"category\":\"" . $entry->getAttribute('category') . "\",\n";
   }

   $index = $entry->getByteOffset() / $addressmapNode->getWordByteSize();
   print $filehandle &getIndent($indent + 1) . "\"index\":" . $index . ",\n";

   @fields = ();
   while (my $field = $entry->getNextFieldNode()) {
      $fieldInfo = {};
      $fieldInfo->{'identifier'} = $field->getIdentifier();
      if ($field->hasDescription()) {
         $fieldInfo->{'decription'} = $field->getDescription();
      }
      if ($field->hasAttribute('type')) {
         $fieldInfo->{'type'} = $field->getAttribute('type');
      }
      if ($field->hasResetValue()) {
         $fieldInfo->{'reset'} = $field->getResetValue();
      }
      $fieldInfo->{'size'} = $field->getWidth();
      push @fields, $fieldInfo;
   }

   print $filehandle &getIndent($indent + 1) . "\"fields\":[";
   $fieldComma = "";
   foreach my $field (@fields) {
      print $filehandle $fieldComma;
      print $filehandle "\"" . $field->{'identifier'} . "\"";
      $fieldComma = ", ";
   }
   print $filehandle "],\n";

   print $filehandle &getIndent($indent + 1) . "\"fielddefs\":{";
   $fieldComma = "";
   foreach my $field (@fields) {
      print $filehandle $fieldComma . "\n";
      &printJsonField($filehandle, ($indent + 2), $field);
      $fieldComma = ",";
   }
   print $filehandle "\n";
   print $filehandle &getIndent($indent + 1) . "}\n";
   print $filehandle &getIndent($indent) . "}";
}

sub printJsonField {
   my ($filehandle, $indent, $field) = @_;
   my ($comma);

   $comma = "";
   print $filehandle &getIndent($indent) .
      "\"" . $field->{'identifier'} . "\":{";

   if (exists($field->{'description'})) {
      print $filehandle $comma . "\n";
      print $filehandle &getIndent($indent + 1) .
         "\"description\":\"" . $field->{'description'} . "\"";
      $comma = ",";
   }

   if (exists($field->{'type'})) {
      print $filehandle $comma . "\n";
      print $filehandle &getIndent($indent + 1) .
         "\"type\":\"" . $field->{'type'} . "\"";
      $comma = ",";
   }

   print $filehandle $comma . "\n";
   print $filehandle &getIndent($indent + 1) .
      "\"size\":\"" . $field->{'size'} . "\"";
   $comma = ",";

   if (exists($field->{'reset'})) {
      print $filehandle $comma . "\n";
      print $filehandle &getIndent($indent + 1) .
         "\"reset\":" . $field->{'reset'};
      $comma = ",";
   }

   print $filehandle "\n";
   print $filehandle &getIndent($indent) . "}";
}

sub getIndent {
   my ($level) = @_;

   return ("   " x $level);
}

sub getPerlPkgName {
   if (!@ARGV) {
      usage();
      die "\n\tERROR - Must specify the Perl package input file name\n\n";
   }
   elsif ($ARGV[0] eq '-h') {
      usage();
      exit(0);
   }

   my $perlPkgName;
   if (@ARGV) {
      $perlPkgName = $ARGV[0];
   }
   else {
      print "\n\tERROR - Must specify Perl package file name!\n\n";
      exit(1);
   }
   if (!-e $perlPkgName) {
      print "\n\tERROR - Perl package file: " . $perlPkgName .
        " does not exist!\n\n";
      exit(1);
   }
   return $perlPkgName;
}

sub usage {
   print STDERR  << "EOF";

     usage: ./perlApiExample.pl [-h] <Perl package file name>

        -h:  prints this message
EOF
}
