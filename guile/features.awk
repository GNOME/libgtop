BEGIN {
	string = "";
	names = "";
}

function output(line) {
	split (line, line_fields, /\|/);
	retval = line_fields[1];
	feature = line_fields[2];
	param_def = line_fields[4];
	
	orig = feature; sub(/^@/,"",feature);
	space = feature; gsub(/./," ",space);

	if (string == "")
		string = " '(";
	else
		string = string"   ";
	string = string"(\""feature"\" (\""retval"\"";

	feature_nounder = feature;
	sub(/_/, "-", feature_nounder);

	if (names == "")
		names = " '(";
	else
		names = names"   ";
	names = names"(\""feature"\" \""feature_nounder"\")\n";
	
	nr_params = split (param_def, params, /:/);
	for (param = 1; param <= nr_params; param++) {
		list = params[param];
		type = params[param];
		sub(/\(.*/, "", type);
		sub(/^\w+\(/, "", list); sub(/\)$/, "", list);

		string = string" (\""type"\"";

		count = split (list, fields, /,/);
		for (field = 1; field <= count; field++) {
			string = string" \""fields[field]"\"";
		}

		string = string")";
	}

	string = string"))\n";
}
	
/^[^#]/		{ output($0) }


END {
	print "(define libgtop-features";
	print string"  )";
	print ")\n";

	print "(define libgtop-feature-names";
	print names"  )";
	print ")\n";
}
