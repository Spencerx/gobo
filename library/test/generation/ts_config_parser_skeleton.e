indexing

	description:

		"Test config parser skeletons"

	library:    "Gobo Eiffel Test Library"
	author:     "Eric Bezault <ericb@gobosoft.com>"
	copyright:  "Copyright (c) 2000-2001, Eric Bezault and others"
	license:    "Eiffel Forum Freeware License v1 (see forum.txt)"
	date:       "$Date$"
	revision:   "$Revision$"

deferred class TS_CONFIG_PARSER_SKELETON

inherit

	YY_PARSER_SKELETON [ANY]
		rename
			make as make_parser_skeleton,
			parse as yyparse
		redefine
			report_error
		end

	TS_CONFIG_SCANNER_SKELETON
		rename
			make as make_config_scanner
		end

	KL_SHARED_FILE_SYSTEM
		export {NONE} all end

feature {NONE} -- Initialization

	make (a_variables: like variables; an_error_handler: like error_handler) is
			-- Create a new config parser.
		require
			a_variables_not_void: a_variables /= Void
			an_error_handler_not_void: an_error_handler /= Void
		do
			variables := a_variables
			make_config_scanner ("unknown file", an_error_handler)
			make_parser_skeleton
		ensure
			variables_set: variables = a_variables
			error_handler_set: error_handler = an_error_handler
		end

feature -- Parsing

	parse (a_file: KI_CHARACTER_INPUT_STREAM) is
			-- Parse Ace file `a_file'.
		require
			a_file_not_void: a_file /= Void
			a_file_open_read: a_file.is_open_read
		do
			reset
			set_input_buffer (new_file_buffer (a_file))
			filename := a_file.name
			last_config := Void
			class_regexp := Void
			feature_regexp := Void
			class_prefix := Void
			compile := Void
			execute := Void
			yyparse
		end

feature -- Access

	last_config: TS_CONFIG
			-- Last config parsed

	class_regexp: LX_REGULAR_EXPRESSION
			-- Class regular expression

	feature_regexp: LX_REGULAR_EXPRESSION
			-- Feature regular expression

	class_prefix: STRING
			-- Prefix of generated testcase class names

	compile: STRING
			-- Compilation command-line

	execute: STRING
			-- Execution command-line

	testgen: STRING
			-- Directory where test classes are generated;
			-- Void means current directory

	variables: TS_VARIABLES
			-- Defined variables

feature -- Status report

	compiler_ise: BOOLEAN
	compiler_hact: BOOLEAN
	compiler_se: BOOLEAN
	compiler_ve: BOOLEAN
			-- Compiler which will be used to compile the test classes

	fail_on_rescue: BOOLEAN
			-- Should the test application crash when an error occur?
			-- (By default test case errors are caught by a rescue
			-- clause and reported to the result summary, but during
			-- debugging it might be useful to get the full exception
			-- trace.)

feature -- Status setting

	set_compiler_ise (b: BOOLEAN) is
			-- Set `compiler_ise' to `b'.
		do
			compiler_ise := b
		ensure
			compiler_ise_set: compiler_ise = b
		end

	set_compiler_se (b: BOOLEAN) is
			-- Set `compiler_se' to `b'.
		do
			compiler_se := b
		ensure
			compiler_se_set: compiler_se = b
		end

	set_compiler_hact (b: BOOLEAN) is
			-- Set `compiler_hact' to `b'.
		do
			compiler_hact := b
		ensure
			compiler_hact_set: compiler_hact = b
		end

	set_compiler_ve (b: BOOLEAN) is
			-- Set `compiler_ve' to `b'.
		do
			compiler_ve := b
		ensure
			compiler_ve_set: compiler_ve = b
		end

	set_fail_on_rescue (b: BOOLEAN) is
			-- Set `fail_on_rescue' to `b'.
		do
			fail_on_rescue := b
		ensure
			fail_on_rescue_set: fail_on_rescue = b
		end

feature -- AST factory

	new_config (a_root_class: STRING; a_testgen: STRING;
		a_compile: STRING; an_execute: STRING;
		a_clusters: DS_LIST [TS_CLUSTER]): TS_CONFIG is
			-- New config
		require
			a_root_class_not_void: a_root_class /= Void
			a_compile_not_void: a_compile /= Void
			an_execute_not_void: an_execute /= Void
			a_clusters_not_void: a_clusters /= Void
			no_void_cluster: not a_clusters.has (Void)
		do
			!! Result.make (a_root_class, a_testgen, a_compile, an_execute, a_clusters)
		ensure
			config_not_void: Result /= Void
		end

	new_cluster (a_name: ET_IDENTIFIER; a_pathname: ET_IDENTIFIER): TS_CLUSTER is
			-- New cluster
		require
			a_name_not_void: a_name /= Void
			a_pathname_not_void: a_pathname /= Void
			class_regexp_not_void: class_regexp /= Void
			feature_regexp_not_void: feature_regexp /= Void
			class_prefix_not_void: class_prefix /= Void
		do
			!! Result.make (a_name, a_pathname, class_regexp, feature_regexp, class_prefix)
		ensure
			cluster_not_void: Result /= Void
		end

	new_regexp (a_regexp: ET_IDENTIFIER): LX_REGULAR_EXPRESSION is
			-- New regular expression;
			-- Void if there is a syntax error in `a_regexp'
		require
			a_regexp_not_void: a_regexp /= Void
		do
			!LX_DFA_REGULAR_EXPRESSION! Result.compile (a_regexp.name, True)
			if not Result.compiled then
				Result := Void
				error_handler.report_regexp_syntax_error (a_regexp)
			end
		ensure
			compiled: Result /= Void implies Result.compiled
		end

feature -- Defaults

	set_defaults (root_class: STRING) is
			-- Set default values if not already set.
		require
			root_class_not_void: root_class /= Void
		do
			if class_regexp = Void then
				class_regexp := Default_class_regexp
			end
			if feature_regexp = Void then
				feature_regexp := Default_feature_regexp
			end
			if class_prefix = Void then
				class_prefix := Default_class_prefix
			end
			if compile = Void then
				compile := default_compile (root_class)
			end
			if execute = Void then
				execute := default_execute (root_class)
			end
		ensure
			class_regexp_not_void: class_regexp /= Void
			feature_regexp_not_void: feature_regexp /= Void
			class_prefix_not_void: class_prefix /= Void
			compile_not_void: compile /= Void
			execute_not_void: execute /= Void
		end

	Default_class_regexp: LX_REGULAR_EXPRESSION is
			-- Default class regexp
		once
			!LX_DFA_REGULAR_EXPRESSION! Result.compile ("TEST_.*", True)
		ensure
			regexp_not_void: Result /= Void
			regexp_compiled: Result.compiled
		end

	Default_feature_regexp: LX_REGULAR_EXPRESSION is
			-- Default feature regexp
		once
			!LX_DFA_REGULAR_EXPRESSION! Result.compile ("test_.*", True)
		ensure
			regexp_not_void: Result /= Void
			regexp_compiled: Result.compiled
		end

	Default_class_prefix: STRING is "X"
			-- Default prefix of generated testcase class names

	default_compile (root_class: STRING): STRING is
			-- Default compilation command-line
		require
			root_class_not_void: root_class /= Void
		do
			if compiler_ise then
				Result := "geant compile_ise"
			elseif compiler_hact then
				Result := "geant compile_hact"
			elseif compiler_se then
				Result := "geant compile_se"
			elseif compiler_ve then
				Result := "geant compile_ve"
			else
				Result := "geant compile"
			end
		ensure
			default_compile_not_void: Result /= Void
		end

	default_execute (root_class: STRING): STRING is
			-- Default execute command-line
		require
			root_class_not_void: root_class /= Void
		local
			a_name: STRING
			a_cursor: DS_HASH_TABLE_CURSOR [STRING, STRING]
		do
			a_name := STRING_.to_lower (root_class) + file_system.exe_extension
			Result := file_system.pathname (file_system.relative_current_directory, a_name)
			if fail_on_rescue then
				Result.append_string (" -a")
			end
			a_cursor := variables.new_cursor
			from a_cursor.start until a_cursor.after loop
				Result.append_string (" -D %"")
				Result.append_string (a_cursor.key)
				Result.append_character ('=')
				Result.append_string (a_cursor.item)
				Result.append_character ('%"')
				a_cursor.forth
			end
		ensure
			default_execute_not_void: Result /= Void
		end

	default_cluster: TS_CLUSTER is
			-- Default cluster
		require
			class_regexp_not_void: class_regexp /= Void
			feature_regexp_not_void: feature_regexp /= Void
			class_prefix_not_void: class_prefix /= Void
		local
			a_name, a_pathname: ET_IDENTIFIER
			a_position: ET_FILE_POSITION
		do
			!! a_position.make ("unknown file", 0, 0)
			!! a_name.make ("root", a_position)
			!! a_pathname.make (".", a_position)
			!! Result.make (a_name, a_pathname, class_regexp, feature_regexp, class_prefix)
		ensure
			cluster_not_void: Result /= Void
		end

feature -- Error handling

	report_error (a_message: STRING) is
			-- Print error message.
		do
			error_handler.report_config_syntax_error (current_position)
		end

invariant

	variables_not_void: variables /= Void
	compiled_class_regexp: class_regexp /= Void implies class_regexp.compiled
	compiled_feature_regexp: feature_regexp /= Void implies feature_regexp.compiled

end -- class TS_CONFIG_PARSER_SKELETON
