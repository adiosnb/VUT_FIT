#!/usr/bin/php
<?php

/**
 * Class Parser - this class checks your input arguments and store value into internal data structure.
 */
class Parser {

    /**
     * Parser constructor.Initialize internal data.
     */
    public function __construct(){
        $this->list_of_short = array();
        $this->list_of_long = array();
        $this->descriptions = array();
        $this->need_to_set = array();
        $this->value_needed = array();
        $this->values = array();
        $this->is_set = array();
        $this->dependencies = array();
        $this->arguments_counter = 0;
    }

    /**
     * This function add new argument into parser.
     * @param $short  short form of argument.
     * @param $long long form of argument.
     * @param $description description which is shown in --help.
     * @param $need_to_set if this argument needs to be set.
     * @param $value_needed if this argument needs value.
     * @param null $default_value if argument no set, but default value is needed.
     */
    public function add_argument($short, $long, $description, $need_to_set, $value_needed, $default_value=null){
        if (!($short == "")){
            $this->list_of_short[$short] =  $this->arguments_counter;
        }

        if (!($long == "")){
            $this->list_of_long[$long] = $this->arguments_counter;
        }

        $this->descriptions[$this->arguments_counter] = $description;
        $this->need_to_set[$this->arguments_counter] = $need_to_set;
        $this->value_needed[$this->arguments_counter] = $value_needed;
        $this->is_set[$this->arguments_counter] = false;
        $this->values[$this->arguments_counter] = $default_value;
        $this->arguments_counter += 1;
    }

    /**
     * This function add dependency into parser, if two of both arguments need to be set.
     * @param $arg This arguments needs second one.
     * @param $need Second one.
     */
    public function add_dependency($arg, $need){
        $this->dependencies[$arg] = $need;
    }

    /**
     * Checks if all statements from dependencies are correct.
     */
    public function check_dependencies(){
        foreach ($this->dependencies as $arg => $needed) {
            if ($this->is_set[$this->get_num_pointer($arg)]){
                if (!($this->is_set[$this->get_num_pointer($needed)])){
                    error(1);
                }
            }
        }
    }

    /**
     * Checks if all arguments, which need value, have value.
     */
    public function check_args(){
        foreach ($this->list_of_short as $key => $value) {
            if ($this->need_to_set[$value]){
                if (!($this->is_set[$value])) {
                    error(1);
                }
            }
        }

        foreach ($this->list_of_long as $key => $value) {
            if ($this->need_to_set[$value]){
                if (!($this->is_set[$value])){

                    error(1);
                }
            }
        }
    }

    /**
     * Parsing all arguments from argv nto data structure.
     * @param $input argv.
     */
    public function parse($input){

        $indian_check = 0;
        foreach ($input as $arg){
            if ($indian_check == 0){
                $indian_check += 1;
                continue;
            }
            $match = false;
            foreach ($this->list_of_short as $short => $value) {
                $is_here = strpos($arg,$short);
                if (!(($is_here === false) or ($is_here > 0))){
                    $str_len = strlen($arg);
                    if ($str_len == 2 ){
                        if ($this->is_set[$value] == true){
                            error(1);
                        } else {
                            $this->is_set[$value] = true;
                            $this->values[$value] = true; # i want to see if set i with __get method
                        }

                        if ($this->value_needed[$value] == true){
                            error(1);
                        }
                        $match = true;
                    } else {
                        if (($arg[2] == "=")){
                            if ($this->is_set[$value] == false){
                                $this->is_set[$value] = true;
                            } else {
                                error(1);
                            }

                            if ($this->value_needed[$value] == false){
                                error(1);
                            }
                            $this->values[$value]=substr($arg, 3);
                        } else {
                            error(1);
                        }
                        $match = true;
                    }
                }
            }

            if ($match == true){
                continue;
            }

            foreach ($this->list_of_long as $long => $value) {
                $is_here = strpos($arg,$long);
                if (!(($is_here === false) or ($is_here > 0))){
                    if (strlen($arg) == strlen($long)){
                        if ($this->is_set[$value] == true){
                            error(1);
                        } else {
                            $this->is_set[$value] = true;
                            $this->values[$value] = true;
                        }

                        if ($this->value_needed[$value] == true){
                            error(1);
                        }
                        $match = true;
                    } else {
                        if ($arg[strlen($long)] == "="){
                            if ($this->is_set[$value] == false) {
                                $this->is_set[$value] = true;
                            } else  {
                                error(1);
                            }

                            if ($this->value_needed[$value] == false) {
                                error(1);
                            }
                            $this->values[$value] = substr($arg, strlen($long) +1);
                        } else {
                            error(1);
                        }
                        $match = true;
                    }

                }
            }

            if ($match == false){
                error(1);
            }
        }
    }

    /**
     * Return set value of argument.
     * @param $key Name fo argument.
     * @return mixed|null Value of argument.
     */
    public function get_value($key){
        foreach ($this->list_of_short as $arg => $value){
            if ($key == $arg){
                return $this->values[$value];
            }
        }

        foreach ($this->list_of_long as $arg => $value) {
            if ($key == $arg) {
                return $this->values[$value];
            }
        }

        return null;
    }

    /**
     * Description of all arguments
     * @param string $prefix Offset of each help message.
     * @return string Help message.
     */
    public function get_help($prefix = ""){
        $return_string = "";
        foreach ($this->descriptions as $description) {
            $return_string = $return_string . $prefix .$description . "\n";
        }
        return $return_string;
    }

    /**
     * Get internal pointer to argument
     * @param $arg Name f argument
     * @return mixed|null Integer pointer to data structure for argument data.
     */
    public function get_num_pointer($arg){
        foreach ($this->list_of_short as $key => $value){
            if ($key == $arg){
                return $value;
            }
        }

        foreach ($this->list_of_long as $key => $value){
            if ($key == $arg){
                return $value;
            }
        }
        return null;
    }

    /**
     * Get interface.
     * @param $name Argument name.
     * @return mixed Value of argument.
     */
    public function __get($name){
        if (strlen($name) == 1){
            foreach ($this->list_of_short as $short => $value){
                if (strpos($short, $name)){
                    return $this->values[$value];
                }
            }
        } else {
            foreach ($this->list_of_long as $long =>  $value) {
                if (strpos($long, $name)){
                    return $this->values[$value];
                }
            }
        }
    }
}

/**
 * Decorative function for recursive encoding tree structure to xml format.
 * @param $parsed_json Tree data structure to encode to XML
 * @param $parser Line arguments.
 * @return string Data encoded to XML.
 */
function xml_encode($parsed_json, $parser){
    $return_xml_string = "";
    $offset = 0;
    if ($parser->n == false){
        $return_xml_string .= "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    }

    # -r="root"
    if (($parser->r != null)) {
        if (!check_key_name($parser->r)){
            error(50);
        }
        $return_xml_string .= "<".$parser->r.">\n";
        $offset++;
    }

    $return_xml_string .= _xml_encode($parsed_json, $parser, $offset);

    # -r="root"
    if (($parser->r != null)){
        $return_xml_string .= "</".$parser->r.">\n";
    }


    return $return_xml_string;
}

/**
 * @param $parsed_json Tree data structure to encode to XML
 * @param null $parser  Line arguments.
 * @param int $offset Offset for making nice and readable output.
 * @param bool $item_tag If array is encoded
 * @param int $padding number of characters to pad in <item  >
 * @return string encoded XML
 */
function _xml_encode($parsed_json, $parser=null, $offset=0, $item_tag=false, $padding=0){
    $return_xml_string = "";

    if (!$item_tag and (is_array($parsed_json))){
        if (is_int(array_keys($parsed_json)[0])){
            $array_name = repair_word($parser->get_value("--array-name"), $parser);
            if (!check_key_name($array_name)){
                error(51);
            }
            $return_xml_string .= make_str_offset($offset) . "<".$array_name;
            $arr_size = sizeof($parsed_json);
            if ($parser->a){
                $return_xml_string .= " size=\"".$arr_size."\"";
            }
            $return_xml_string .= ">\n";

            $padding_size_atr = $arr_size + $parser->get_value("--start");

            $return_xml_string .= _xml_encode($parsed_json,$parser,$offset+1,true, $padding_size_atr);
            $return_xml_string .= make_str_offset($offset) . "</".$array_name.">\n";
            return $return_xml_string;
        }
    }

    $i=$parser->get_value("--start");
    foreach ($parsed_json as $key => $value) {
        if (is_array($value) or is_object($value)){
            if (is_int($key)) {
                $item_name = repair_key($parser->get_value("--item-name"), $parser);
                if (!(check_key_name($item_name))){
                    error(51);
                }
                $return_xml_string .= make_str_offset($offset)."<".$item_name;
                if ($parser->t){
                    if ($parser->padding){
                        $num_len = strlen(sizeof($parsed_json) + $parser->get_value("--start"));
                        $padded_index = str_pad($i, $num_len, "0", STR_PAD_LEFT);
                        $return_xml_string .= " index=\"".$padded_index."\"";
                    } else {
                        $return_xml_string .= " index=\"".$i."\"";
                    }
                }
                $return_xml_string .=">\n";
                $return_xml_string .= _xml_encode($value, $parser, $offset+1);
                $return_xml_string .= make_str_offset($offset)."</".$item_name.">\n";

            } else {
                $key = repair_key($key, $parser);
                if (!check_key_name($key)){
                    error(51);
                }
                $return_xml_string .= make_str_offset($offset)."<".$key.">\n";
                $return_xml_string .= _xml_encode($value, $parser, $offset + 1);
                $return_xml_string .= make_str_offset($offset)."</".$key.">\n";
            }
        } else {
            if (is_int($key) and $parser->t){
                $return_xml_string .= encode_data($key, $value, $parser, $offset, $i , $padding);
            } else {
                $return_xml_string .= encode_data($key, $value, $parser, $offset);
            }
        }
        $i++;
    }

    return $return_xml_string;
}

/**
 * Encode element pair to XML
 * @param $key Key element name.
 * @param $value Value to encode.
 * @param $parser Line arguments.
 * @param int $offset Make nice output.
 * @param null $index counter for array.
 * @param int $padding number of characters to pad in <item  index="xxx">
 * @return string Encoded XML
 */
function encode_data($key, $value, $parser, $offset=0, $index=null, $padding=0){
    $return_string = "";
    $key_first = decode_word($key, $parser);
    $orig_value = $value;
    $value = decode_word($value, $parser);
    $value = repair_word($value, $parser);


    if (is_int($key_first)){
        $key_first = $parser->get_value("--item-name");
    }

    $key_first = repair_key($key_first, $parser);

    if (!(check_key_name($key_first))){
        error(51);
    }
    $key_second = $key_first;

    if ($index != null){
        if ($parser->padding){
            $num_len = strlen($padding);
            $padded_index = str_pad($index, $num_len, "0", STR_PAD_LEFT);
            $key_first .= " index=\"" . $padded_index . "\"";
        } else {
            $key_first .= " index=\"" . $index . "\"";
        }
    }
    if (is_bool($orig_value) or is_null($orig_value)){
        if ($parser->l){
            $return_string .= make_str_offset($offset)."<".$key_first;
            if ($parser->get_value("--types") == true) {
                $return_string .= " type=\"literal\"";
            }
            $return_string .= ">";
            $return_string .="<".$value."/>";
            $return_string .= "</".$key_second.">\n";
            return $return_string;
        } else {
            $return_string .= make_str_offset($offset)."<".$key_first." value=\"".$value."\"";
            if ($parser->get_value("--types")) {
                $return_string .= " type=\"literal\"";
            }
            $return_string .= "/>\n";
        }
    }
    elseif (is_int($value)){

        if ($parser->i){
            $return_string .= make_str_offset($offset)."<".$key_first;
            if ($parser->get_value("--types")) {
                $return_string .= " type=\"integer\"";
            }
            $return_string .= ">";
            $return_string .= $value;
            $return_string .= "</".$key_second.">\n";
            return $return_string;
        } else {
            $return_string .= make_str_offset($offset)."<".$key_first." value=\"".$value."\"";
            if ($parser->get_value("--types")) {
                $return_string .= " type=\"integer\"";
            }
            $return_string .= "/>\n";
        }
    }
    elseif (is_float($value)){
        $number = floor($value);
        if ($parser->i){
            $return_string .= make_str_offset($offset)."<".$key_first;
            if ($parser->get_value("--types")) {
                $return_string .= " type=\"real\"";
            }
            $return_string .= ">";
            $return_string .= $number;
            $return_string .= "</".$key_second.">\n";
            return $return_string;
        }
        else {
            $return_string .= make_str_offset($offset)."<".$key_first." value=\"".$number."\"";
            if ($parser->get_value("--types")) {
                $return_string .= " type=\"real\"";
            }
            $return_string .= "/>\n";
        }
    }
    else {
        if ($parser->s){
            $return_string .= make_str_offset($offset)."<".$key_first;
            if ($parser->get_value("--types")) {
                $return_string .= " type=\"string\"";
            }
            $return_string .= ">";
            $return_string .= $value;
            $return_string .= "</".$key_second.">\n";
            return $return_string;
        } else {
            $return_string .= make_str_offset($offset)."<".$key_first." value=\"".$value."\"";
            if ($parser->get_value("--types")) {
                $return_string .= " type=\"string\"";
            }
            $return_string .= "/>\n";
        }
    }

    return $return_string;
}

/**
 * Return n times 4 spaces
 * @param $width number of spaces.
 * @return string String offset.
 */
function make_str_offset($width){
    $offset="";
    for ($i = 0 ; $i < $width; $i++){
        $offset .= "    ";
    }
    return $offset;
}

/**
 * Return decoded value.
 * @param $word value to decode
 * @param $parser Line arguments.
 * @return string decoded value.
 */
function decode_word($word, $parser){
    if ($word === null){
        return "null";
    } elseif (is_bool($word)){
        if ($word){
            return "true";
        } else {
            return "false";
        }
    }
    return $word;
}

/**
 * Delete bad characters from element name according to line arguments.
 * @param $word element name.
 * @param $parser line arguments
 * @return string Repaired name.
 */
function repair_key($word, $parser) {
    $word = repair_word($word, $parser);
    $ret_word = "";

    $char = mb_substr($word,0,1);

    if (ctype_alpha($char) or $char == "_"){
        $ret_word .= $char;
    } else {
        $ret_word .= $parser->h;
    }

    for ($i = 1 ; $i < mb_strlen($word); $i++){
        $char = mb_substr($word,$i,1);

        if ((ord($char) > 127) or
            ($char == "-") or
            ($char == "_") or
            ($char == ".") or
            ($char == ":") or
            (ctype_digit($char)) or
            (ctype_alpha($char))) {
            $ret_word .= $char;
        } else {
            $ret_word .= $parser->h;
        }
    }
    return $ret_word;
}

/**
 * Repair value to be correct in XML structere.
 * @param $word value to repair
 * @param $parser line arguments
 * @return string repaired value
 */
function repair_word($word, $parser){
    if (is_float($word) or is_int($word)){
        return $word;
    }

    $subst = $parser->h;
    $word_len = mb_strlen($word);
    $correct_word = "";

    for ( $i = 0 ; $i < $word_len; $i++){
        $char = mb_substr($word,$i,1);

        if ($char == "&"){
            if ($parser->c) {
                $correct_word .= "&amp;";
                continue;
            } else {
                $correct_word .= $subst;
                continue;
            }
        }
        elseif ($char == "<") {
            if ($parser->c) {
                $correct_word .= "&lt;";
                continue;
            } else {
                $correct_word .= $subst;
                continue;
            }
        }
        elseif ($char == ">"){
            if ($parser->c) {
                $correct_word .= "&gt;";
                continue;
            } else {
                $correct_word .= $subst;
                continue;
            }
        }
        elseif ( $char == "\""){
            if ($parser->c) {
                $correct_word .= "&quot;";
                continue;
            } else {
                $correct_word .= $subst;
                continue;
            }
        }
        elseif ( $char == "'"){
            if ($parser->c) {
                $correct_word .= "&apos;";
                continue;
            } else {
                $correct_word .= $subst;
                continue;
            }
        }
        else {
            $correct_word .= $char;
        }
    }


    return $correct_word;
}

/**
 * Check if element name is correct according to XML standards.
 * @param $word alement name
 * @return bool result
 */
function check_key_name($word){

    if ($word[0] == "x" or $word[0] == "X"){
        if ($word[1] == "m" or $word[1] == "M") {
            if ($word[2] == "l" or $word[2] == "L"){
                return false;
            }
        }
    }

    if (!(ctype_alpha($word[0]) or $word[0] == "_" or (ord($word[0]) > 127))){
        return false;
    }

    if (ctype_digit($word[0])){
        return false;
    }

    for ($i = 0 ; $i < mb_strlen($word); $i++){
        $char = mb_substr($word,$i,1);

        if (ord($char) > 127) continue;
        if ($char == "-") continue;
        if ($char == "_") continue;
        if ($char == ".") continue;
        if ($char == ":") continue;
        if (ctype_digit($char)) continue;
        if (ctype_alpha($char)) continue;

        return false;
    }


    return true;
}


function error($error){
    #print "ERROR " . $error . "\n";
    #throw new ErrorException("asdfsadf", $error);
    exit($error);
}

function print_help($arg_str){
    print $arg_str;
}

/**
 * Main body of program. Here is Parser initialized, arguments are checked, file are handled and first call of xml_encode().
 * @param $argc
 * @param $argv
 */
function main($argc, $argv)
{

    $parser = new Parser();
    $parser->add_argument("", "--help", "--help  \t\tWrites this help message.", false, false);
    $parser->add_argument("", "--input", "--input=filename  \tInput JSON file in UTF-8 coding.", false, true, "php://stdin");
    $parser->add_argument("", "--output", "--output=filename  \tText output XML file in UTF-8 coding containing JSON values.", false, true, "php://stdout");
    $parser->add_argument("-h", "", "-h=subst \t\tReplace every forbidden character with subst str. Default : \"-\".", false, true, "-");
    $parser->add_argument("-n", "", "-n \t\t\tNot generating XML header to output script.", false, false, false);
    $parser->add_argument("-r", "", "-r=root-element  \tName of root pair element witch contains element.", false, true);
    $parser->add_argument("", "--array-name", "--array-name=array-element  \tRename element wrapping array with implicit values array to array-element.", false, true, "array");
    $parser->add_argument("", "--item-name", "--item-name=item-element  \tRenaming element name for items of array.", false, true, "item");
    $parser->add_argument("-s", "", "-s  \t\t\tString type values will be transformed to text elements.", false, false);
    $parser->add_argument("-i", "", "-i  \t\t\tNumber type values will be transformed to text elements.", false, false);
    $parser->add_argument("-l", "", "-l \t\t\tLiteral values (true, false, null) will be transformed to elements <true/>, <false/> a <null/>.", false, false);
    $parser->add_argument("-c", "", "-c \t\t\tEnable translate problem characters.", false, false);
    $parser->add_argument("-a", "--array-size", "-a, --array-size \tAdd size attribute in arrays.", false, false);
    $parser->add_argument("-t", "--index-items", "-t, --index-items \tAdd index attribute to every item of array. Begin from 1 or --start=n.", false, false);
    $parser->add_argument("", "--start", "--start=n  \t\tInitialize counter for array indexing. Need to combine with --index-item. Default : \"1\".", false, true, 1);
    $parser->add_argument("", "--types", "--types \t\tAdd wrapping element of every scalar value containing type.", false, false);
    $parser->add_argument("", "--padding", "--padding\t\tPadding counter in array. Filling with zeros.", false, false);

    $parser->add_dependency("--start", "-t");
    $parser->add_dependency("--padding", "-t");

    $parser->parse($argv);
    $parser->check_args();
    $parser->check_dependencies();

    if ($parser->help) {
        print_help($parser->get_help());

        foreach ($parser->list_of_short as $key => $value) {
            if (($parser->is_set[$value])) {
                error(1);
            }
        }

        foreach ($parser->list_of_long as $key => $value) {
            if ($key != "--help"){
                if (($parser->is_set[$value])){
                    error(1);
                }
            }
        }
        exit(0);
    }

    if(!ctype_digit($parser->start) and $parser->is_set[$parser->get_num_pointer("--start")]){
        error(1);
    }

    if ($parser->is_set[$parser->get_num_pointer("--array-name")]){
        if (!check_key_name($parser->get_value("--array-name"))){
            error(50);
        }
    }

    if ($parser->is_set[$parser->get_num_pointer("--item-name")]){
        if (!check_key_name($parser->get_value("--item-name"))){
            error(50);
        }
    }


    # opening input file
    #$input_file = fopen($parser->input, "r");
    $input_file = file_get_contents($parser->input);
    if ($input_file === false) {
        # TODO
        error(2);
    }

    # opening output file
    $output_file = fopen($parser->output, "w");
    if ($output_file === false){
        error(3);
    }

    #decoding json
    $input_json_parsed = json_decode($input_file);
    if ($input_json_parsed === null and json_last_error() !== JSON_ERROR_NONE) {
        error(4);
    }

    #var_dump($input_json_parsed);

    fwrite($output_file,xml_encode($input_json_parsed, $parser));
}
main($argc, $argv);