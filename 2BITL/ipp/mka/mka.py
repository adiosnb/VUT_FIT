#!/bin/python36

import argparse
import pprint
import sys
import copy

# constants for argument parser
DESCRIPTION = """
This program read automata, checks if it is good specified automata and minimize it if required"""
EPILOG = """
Examples of usage :
python36 mka.py  -m --input mka-supplementary-tests/test02.in
"""
MAX_ARG = 1

# tokens used in parser
# this is solution is maybe slower but much easier to debug
LEFT_SINGLE_PARENTHESIS = "LEFT_SINGLE_PARENTHESIS"
RIGHT_SINGLE_PARENTHESIS = "RIGHT_SINGLE_PARENTHESIS"
LEFT_MULTI_PARENTHESIS = "LEFT_MULTI_PARENTHESIS"
RIGHT_MULTI_PARENTHESIS = "RIGHT_MULTI_PARENTHESIS"
COMMA = "COMMA"
ID = "ID"
LETTER = "LETTER"
ARROW = "ARROW"
EPSILON = "EPSILON"


def parse_line_arguments():
    """
    Parser arguments from cmd line and do basic check. Max number of putted arguments are 1. There is one
    group of conflict arguments listed in mutually exclusive group.
    :return: Data structure with parsed arguments
    """

    parser = argparse.ArgumentParser(description=DESCRIPTION, epilog=EPILOG, add_help=False)
    group = parser.add_mutually_exclusive_group()
    group.add_argument('-f', '--find-non-finishing', action='count', default=0,
                       help='find non finishing state in automata')
    group.add_argument('-m', '--minimize', action='count', default=0,
                       help='minimize read state machine')
    parser.add_argument('--input', action='append', default=[],
                        help='specify input text file in UTF-8 with good specified automata')
    parser.add_argument('--output', action='append', default=[],
                        help='specify output file for equivalent for input automata')
    parser.add_argument('-i', '--case-insensitive', action='count', default=0,
                        help='not care about size of letters, huge''letters will be converted to small')
    parser.add_argument('--help', action='count', default=0, help='this will print thishelp message')
    try:
        args = parser.parse_args()

    except SystemExit:
        exit(1)

    if args.help:
        parser.print_help()
        if (args.minimize or
            args.find_non_finishing or
            args.case_insensitive or
            len(args.output) or
                len(args.input)):
            exit(1)
        else:
            exit(0)
    if (args.minimize > MAX_ARG or
        args.find_non_finishing > MAX_ARG or
        args.case_insensitive > MAX_ARG or
        len(args.input) > MAX_ARG or
            len(args.output) > MAX_ARG):
        try:
            parser.error("Too many arguments")
        except SystemExit:
            exit(1)

    return args


class AutomataParserError(Exception):
    pass


class AutomataSemanticError(Exception):
    pass


class AutomataNotGoodSpec(Exception):
    pass

class AutomataEndOfSplitting(Exception):
    pass


class AutomataParser:

    def __init__(self, input_string):
        self._input_file = input_string
        self._file_len = len(input_string)
        self._position_in_file = 0
        self._id_str = ""

        self.automata = Automata()

    def get_id(self):
        return self._id_str

    def get_char(self):
        if self._position_in_file == self._file_len:
            raise EOFError
        current_char = self._input_file[self._position_in_file:self._position_in_file + 1]
        self._position_in_file += 1
        return current_char

    def put_back_char(self):
        self._position_in_file -= 1

    def get_token(self):
        current_char = self.get_char()

        while current_char.isspace():
            current_char = self.get_char()
        # print(current_char, end='')

        if current_char == '#':
            while current_char != '\n':
                current_char = self.get_char()
            return self.get_token()

        if current_char == '(':
            return LEFT_SINGLE_PARENTHESIS
        elif current_char == ')':
            return RIGHT_SINGLE_PARENTHESIS
        elif current_char == '{':
            return LEFT_MULTI_PARENTHESIS
        elif current_char == '}':
            return RIGHT_MULTI_PARENTHESIS
        elif current_char == ',':
            return COMMA

        elif current_char == "'":
            self._id_str = ""
            # TODO : think about ''''
            self._id_str += current_char
            current_char = self.get_char()

            if current_char == "'":
                self._id_str += current_char
                current_char = self.get_char()
                if current_char == "'":
                    self._id_str += current_char
                    current_char = self.get_char()
                    if current_char == "'":
                        self._id_str += current_char
                        return LETTER  # TODO : test
                raise AutomataParserError
            else:
                self._id_str += current_char
                current_char = self.get_char()
                if current_char == "'":
                    self._id_str += current_char
                    return LETTER
                else:
                    raise AutomataParserError

        elif current_char == "-":
            current_char = self.get_char()
            if current_char == '>':
                return ARROW
            else:
                raise AutomataParserError
        elif current_char == "_":
            raise AutomataParserError
        else:
            self._id_str = ""
            while current_char.isalpha() or current_char.isdigit() or current_char == '_':
                self._id_str += current_char
                current_char = self.get_char()
            self.put_back_char()
            if self._id_str[len(self._id_str) - 1] == "_":
                raise AutomataParserError
            return ID

    def parse_file(self):
        self._parse_automata()
        return copy.deepcopy(self.automata)

    def _parse_automata(self):
        token = self.get_token()
        if token != LEFT_SINGLE_PARENTHESIS:
            raise AutomataParserError

        self._parse_states()

        token = self.get_token()
        if token != COMMA:
            raise AutomataParserError

        self._parse_chars()

        token = self.get_token()
        if token != COMMA:
            raise AutomataParserError

        self._parse_rules()

        token = self.get_token()
        if token != COMMA:
            raise AutomataParserError

        self._parse_init_state()

        token = self.get_token()
        if token != COMMA:
            raise AutomataParserError

        self._parse_end_states()

        token = self.get_token()
        if token != RIGHT_SINGLE_PARENTHESIS:
            raise AutomataParserError

    def _parse_states(self):
        if self.get_token() != LEFT_MULTI_PARENTHESIS:
            raise AutomataParserError

        token = self.get_token()
        while token != RIGHT_MULTI_PARENTHESIS:
            if token == ID:
                tmp_str = copy.deepcopy(self._id_str)
                self.automata.add_state(tmp_str)
                token = self.get_token()
            elif token == COMMA:
                token = self.get_token()
                if token == RIGHT_MULTI_PARENTHESIS:
                    raise AutomataParserError
            else:
                raise AutomataParserError

    def _parse_chars(self):
        if self.get_token() != LEFT_MULTI_PARENTHESIS:
            raise AutomataParserError

        token = self.get_token()
        while token != RIGHT_MULTI_PARENTHESIS:
            if token == LETTER:
                tmp_str = copy.deepcopy(self._id_str)
                self.automata.add_letter(tmp_str)
                token = self.get_token()
            elif token == COMMA:
                token = self.get_token()
                if token == RIGHT_MULTI_PARENTHESIS:
                    raise AutomataParserError
            else:
                raise AutomataParserError

    def _parse_rules(self):
        if self.get_token() != LEFT_MULTI_PARENTHESIS:
            raise AutomataParserError

        token = self.get_token()
        while token != RIGHT_MULTI_PARENTHESIS:
            if token != ID:
                raise AutomataParserError
            state = copy.deepcopy(self._id_str)
            if self.get_token() != LETTER:
                raise AutomataParserError
            letter = copy.deepcopy(self._id_str)
            if self.get_token() != ARROW:
                raise AutomataParserError
            if self.get_token() != ID:
                raise AutomataParserError
            next_state = copy.deepcopy(self._id_str)
            self.automata.add_rule(state, letter, next_state)
            token = self.get_token()
            if token == COMMA:
                token = self.get_token()
                if token == RIGHT_MULTI_PARENTHESIS:
                    raise AutomataParserError

    def _parse_init_state(self):
        if self.get_token() != ID:
            raise AutomataParserError
        self.automata.set_init_state(copy.deepcopy(self._id_str))

    def _parse_end_states(self):
        if self.get_token() != LEFT_MULTI_PARENTHESIS:
            raise AutomataParserError

        token = self.get_token()
        while token != RIGHT_MULTI_PARENTHESIS:
            if token != ID:
                raise AutomataParserError
            end_state = copy.deepcopy(self._id_str)
            self.automata.add_end_state(end_state)
            token = self.get_token()
            if token == COMMA:
                token = self.get_token()
                if token == RIGHT_MULTI_PARENTHESIS:
                    raise AutomataParserError


class Automata:
    """
    This is class representing finit state machine. After init() is empty (no states, no rules, etc.),
    so you have to add states, letters, rules, init_state and end states. After filling up all variables,
    you should call functions for checking consistency of automata. If automata is good specified, you
    can use function for minimizing of automata.
    """

    def __init__(self):
        self.states = []
        self.letters = []
        self.rules = []
        self.end_states = []
        self.start_state = None

    def add_state(self, id):
        if id not in self.states:
            self.states.append(id)

    def add_letter(self, id):
        if id not in self.letters:
            self.letters.append(id)

    def add_rule(self, state, letter, next_state):
        """
        Creates tuple, which represents rule and add it into rules list.
        :param state: start_state
        :param letter: Symbol which is required for step.
        :param next_state: end_statte
        :return:
        """
        if (state, letter, next_state) not in self.rules:
            self.rules.append((state, letter, next_state))

    def set_init_state(self, id):
        self.start_state = id

    def add_end_state(self, id):
        if id not in self.end_states:
            self.end_states.append(id)

    def __str__(self):
        """
        Creates nice readable output of automata. This format is required in taks.
        :return: Automata in String
        """
        self.states.sort()
        self.rules.sort()
        self.end_states.sort()

        return_string = "(\n{"
        for state in self.states:
            return_string += state + ", "
        return_string = return_string[:-2]
        return_string += "},\n{"

        for letter in self.letters:
            return_string += letter + ", "
        return_string = return_string[:-2]
        return_string += "},\n{\n"
        for state, letter, next_state in self.rules:
            return_string += "%s %s -> %s,\n" % (state, letter, next_state)
        return_string = return_string[:-2]
        return_string += "\n},\n"

        return_string += self.start_state + ",\n{"

        for end_state in self.end_states:
            return_string += end_state + ", "
        if len(self.end_states) > 0:
            return_string = return_string[:-2]
        return_string += "}\n)\n"

        return return_string

    def check_automata(self):
        """
        Check if there is samo semantic error. If there is any mistake, it will raise an AutomataSemanticError exception.
        """
        if len(self.letters) == 0:
            raise AutomataSemanticError

        if self.start_state not in self.states:
            raise AutomataSemanticError

        for state, letter, next in self.rules:
            if state not in self.states:
                raise AutomataSemanticError
            if letter not in self.letters:
                raise AutomataSemanticError
            if next not in self.states:
                raise AutomataSemanticError

        for end_state in self.end_states:
            if end_state not in self.states:
                raise AutomataSemanticError

    def to_lower(self):
        """
        Converts every letter in state ID to lower letter. This is solution for case-insesitive
        function of this automata application.
        :return:
        """
        for i, state in enumerate(self.states):
            self.states[i] = state.lower()

        for i, letter in enumerate(self.letters):
            self.letters[i] = letter.lower()

        for i, rule in enumerate(self.rules):
            state, letter, next = rule
            self.rules[i] = (state.lower(), letter.lower(), next.lower())

        self.start_state = self.start_state.lower()

        for i, end_state in enumerate(self.end_states):
            self.end_states[i] = end_state.lower()

    def is_good_specified(self):
        """
        Check if automata is well specified. If there is any mistake, it will raise an AutomataNotGoodSpec exception.
        """
        # checking determinitic
        state_letter = [(state, letter) for state, letter, next_state in self.rules]
        if len(state_letter) != len(set(state_letter)):
            raise AutomataNotGoodSpec
        # searching epsilon rule
        # TODO

        if len(self.find_non_finishing()) > 1:
            raise AutomataNotGoodSpec

        # check if is complete
        for state in self.states:
            for letter in self.letters:
                if (state, letter) not in state_letter:
                    raise AutomataNotGoodSpec

        # finding inaccessible states
        for current_state in self.states:
            if not self.path_exist(self.start_state, current_state):
                raise AutomataNotGoodSpec

    def find_non_finishing(self):
        """
        Find every non finishing state ina automata.
        :return: List od those states.
        """
        non_finishing = []
        for state in self.states:
            for end_state in self.end_states:
                if self.path_exist(state, end_state):
                    break
            else:  # NO BREAK
                non_finishing.append(state)
        return non_finishing

    def path_exist(self, start, end):
        """
        Finds if there is some path from start to end. BFS algorithm of AI is used.
        :param start: From this state algorithm start.
        :param end: This state will be found.
        :return: BOOL if path was founded.
        """
        open_queue = [start]
        closed_queue = []
        while len(open_queue) > 0:
            current_state = open_queue.pop(0)
            if current_state in closed_queue:
                continue
            closed_queue.append(current_state)
            for state, letter, next_state in self.rules:
                if state == current_state and next_state not in closed_queue:
                    open_queue.append(next_state)
                    if end == next_state:
                        return True

        return end in closed_queue

    def minimize(self):
        # Qm = {{p:p e F},{q:q e Q-F}};
        Qm = []
        Qm.append(self.end_states)
        Qm.append([i for i in self.states if i not in self.end_states])

        try:
            while True:
                Qm = self.split_sets_of_states(Qm)
        except AutomataEndOfSplitting:
            pass

        new_states = [self.create_new_state(list(states)) for states in Qm]

        new_rules = set()

        for state_set in Qm:
            for symbol in self.letters:
                for state, letter, next_state in self.rules:
                    if state in state_set and letter == symbol:
                        tmp_state = [self.create_new_state(list(x)) for x in Qm if state in x][0]
                        tmp_next_state = [self.create_new_state(list(x)) for x in Qm if next_state in x][0]
                        new_rules.add((tmp_state, letter, tmp_next_state))

        new_start_state = [self.create_new_state(x) for x in Qm if self.start_state in x][0]

        new_end_states = set()
        for x in Qm:
            for end_state in self.end_states:
                if end_state in x:
                    new_end_states.add(self.create_new_state(x))

        new_states.sort()
        self.states = new_states
        new_rules = list(new_rules)
        new_rules.sort()
        self.rules = new_rules
        self.start_state = new_start_state
        new_end_states = list(new_end_states)
        new_end_states.sort()
        self.end_states = new_end_states

    def split_sets_of_states(self, sets_of_states):
        for state_set in sets_of_states:
            for letter in self.letters:
                next_states = set()
                rules_for_letter = []
                for state, rule_letter, next_state in self.rules:
                    if rule_letter == letter and state in state_set:
                        next_states.add(next_state)
                        rules_for_letter.append((state, rule_letter, next_state))
                if len(next_states) > 0:
                    next_states = list(next_states)
                    for check_set in sets_of_states:
                        if self.is_sublist(next_states, check_set):
                            break
                    else:  # NO BREAK
                        # start spliting
                        X1 = set()
                        X2 = set()
                        X1.add(rules_for_letter[0][0])
                        Q1 = [rules_for_letter[0][2]]
                        Q1_set = [q_set for q_set in sets_of_states if Q1[0] in q_set]
                        Q1_set = Q1_set[0]
                        for state, rule_letter, next_state in rules_for_letter:
                            if next_state in Q1_set:
                                X1.add(state)
                            else:
                                X2.add(state)
                        new_qm_set = [q for q in sets_of_states if q is not state_set]
                        new_qm_set.append(list(X1))
                        new_qm_set.append(list(X2))
                        return new_qm_set
        raise AutomataEndOfSplitting

    @staticmethod
    def is_sublist(list_a, list_b):
        for item in list_a:
            if item not in list_b:
                return False
        return True

    @staticmethod
    def create_new_state(state_list):
        state_list.sort()
        new_name = "%s" % state_list.pop(0)
        for state in state_list:
            new_name += "_%s" % state
        return new_name


if __name__ == "__main__":
    args = parse_line_arguments()

    try:
        if len(args.input):
            file_handler = open(args.input[0])
            read_file = file_handler.read()
            file_handler.close()
        else:
            read_file = sys.stdin.read()

        parser = AutomataParser(read_file)
        automata = parser.parse_file()

        if args.case_insensitive:
            automata.to_lower()

        automata.check_automata()
        automata.is_good_specified()

        if args.find_non_finishing:
            non_finishing = automata.find_non_finishing()
            if len(non_finishing):
                out = non_finishing[0]
            else:
                out = "0"

        if args.minimize:
            automata.minimize()

        if not args.find_non_finishing:
            out = str(automata)

        if len(args.output):
            file_handler = open(args.output[0], 'w')
            file_handler.write(out)
            file_handler.close()
        else:
            read_file = sys.stdout.write(out)

    except AutomataParserError:
        sys.stderr.write("Syntax error\n")
        exit(60)
    except AutomataSemanticError:
        sys.stderr.write("Semantic error\n")
        exit(61)
    except AutomataNotGoodSpec as e:
        sys.stderr.write("No good specified automata\n")
        exit(62)
