<?
// XML_Search 1.0 - diet-XPath for PHP
//
// Copyright (C) 2003 by Jarno Elonen <elonen@iki.fi>
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// + Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// + Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// + The name of the author may not be used to endorse or promote products derived
//   from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ''AS IS'' AND ANY EXPRESS OR IMPLIED
// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR
// BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  function xml_search( $filename, $searches )
  {
    // Parses an XML file with SAX, applies given searches
    // to it on the fly and returns the corresponding nodes,
    // if found.
    //
    // $searches is an array that contains search
    // paths indexed with keys under which the nodes
    // will be mapped in the result.
    //
    // For example, if $searches contained...
    //   "name" => "movie/info/title@lang=en",
    // ...the function would look for...
    //   <movie><info><title lang="en">DATA</title></info></movie>
    // ...and return it in the result array as:
    //   "name" => Array( "n"=>"TITLE", "v"=>"data", "a"=>Array("LANG"=>"en"))
    //
    // There can be any number of attribute equality tests,  for example:
    //    "movie@style=action/info/title@lang=en@director=James Cameron@year=1994"
    //
    // Limitations: NOT re-entrant, no wildcards,
    // only equality test for attributes

    global $xmls_searches, $xmls_results, $xmls_elemstack;
    $xmls_searches = $searches;
    $xmls_elemstack = Array();
    $xmls_results = Array();

    set_time_limit( 20 );
    $xml_parser = xml_parser_create();
    xml_set_element_handler($xml_parser, "xmls_start_element", "xmls_end_element");
    xml_set_character_data_handler($xml_parser, "xmls_character_data");
    if (!($fp = fopen($filename, "r")))
        die("could not open XML input");
    while ($data = fread($fp, 4096))
        if (!xml_parse($xml_parser, $data, feof($fp)))
            die(sprintf("XML error: %s at line %d",
                        xml_error_string(xml_get_error_code($xml_parser)),
                        xml_get_current_line_number($xml_parser)));
    @fclose($fp);
    xml_parser_free($xml_parser);
    return $xmls_results;
  }

  $xmls_searches = Array();
  $xmls_results = Array();
  $xmls_elemstack = Array();

  function xmls_start_element($parser, $name, $attrs)
  {
      global $xmls_elemstack;
      array_push($xmls_elemstack, Array( "n"=>$name, "a"=>$attrs, "v"=>""));
  }

  function xmls_end_element($parser, $name)
  {
    global $xmls_elemstack, $xmls_searches, $xmls_results;
    reset( $xmls_searches );
    while( list($path_key, $path) = each($xmls_searches))
    {
      $cmp = explode("/", $path);
      if ( count($cmp) !== count($xmls_elemstack))
        break;

      $found = True;
      reset( $xmls_elemstack );
      while( list(,$node) = each( $xmls_elemstack ))
      {
        $c = explode( "@", current($cmp));
        next($cmp);
        if ( strcasecmp($c[0], $node["n"]) != 0 )
        {
          $found = False;
          break;
        }
        for ( $i=1; $i<count($c); ++$i )
        {
          list($attr_k, $attr_v) = explode("=", $c[$i], 2);
          $attr_k = strtoupper($attr_k);
          if ( !isset($node["a"][$attr_k]) || $node["a"][$attr_k] !== $attr_v )
          {
            $found = False;
            break;
          }
        }
      }
      if ( $found && each( $xmls_elemstack ) === False )
      {
        $node =& end($xmls_elemstack);
        $xmls_results[$path_key] = $node;
        unset($xmls_searches[$path_key]);
      }
    }
    array_pop($xmls_elemstack);
  }

  function xmls_character_data($parser, $data)
  {
    global $xmls_elemstack;
    end($xmls_elemstack);
    $xmls_elemstack[key($xmls_elemstack)]["v"] .= $data;
  }
?>