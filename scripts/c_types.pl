# Internal functions

my $c_strlen_func = sub {
  local ($_) = @_;

  return "strlen ($_) + 1";
};

my $c_marshal_func = sub {
  my ($type, $param, $indent) = @_;

  my $code = '';
  $code .= sprintf ("%s_LIBGTOP_SEND_temp_len = strlen (%s)+1;\n",
		    $indent, $param);
  $code .= sprintf ("%smemcpy (_LIBGTOP_SEND_ptr, %s, %s);\n",
		    $indent, "&_LIBGTOP_SEND_temp_len", "sizeof (size_t)");
  $code .= sprintf ("%s_LIBGTOP_SEND_ptr += sizeof (size_t);\n", $indent);
  $code .= sprintf ("%smemcpy (_LIBGTOP_DATA_ptr, %s, %s);\n",
		    $indent, $param, "strlen ($param)+1");
  $code .= sprintf ("%s_LIBGTOP_DATA_ptr += strlen ($param)+1;\n",
		    $indent);

  $need_temp_len = 1;

  return $code;
};

my $c_demarshal_func = sub {
  my ($type, $param, $indent) = @_;

  my $code = '';
  $code .= sprintf ("%s_LIBGTOP_demarshal_%s = _LIBGTOP_DATA_ptr;\n",
		    $indent, $param);
  $code .= sprintf ("%sif (_LIBGTOP_TEMP_len) --_LIBGTOP_TEMP_len;\n",
		    $indent);
  $code .= sprintf ("%s*(_LIBGTOP_DATA_ptr + _LIBGTOP_TEMP_len) = 0;\n",
		    $indent);

  $need_temp_len = 1;

  return $code;
};



# Typeinfo array fields:
# ---------------------
# * C type name
# * Flag specifying whether we need to copy the parameter into temp storage
#

$typeinfo = {'long'	=> ['gint64',		0],
	     'ulong'	=> ['guint64',	0],
	     'pid_t'	=> ['pid_t',		0],
	     'int'	=> ['int',		0],
	     'retval'	=> ['int',		0],
	     'ushort'	=> ['unsigned short',	0],
	     'unsigned'	=> ['unsigned',		0],
	     'string'	=> ['const char *',	1],
	     };

$sizeof_funcs = {'string'	=> $c_strlen_func,
		};

$marshal_funcs = {'string'	=> $c_marshal_func,
		 };

$demarshal_funcs = {'string'	=> $c_demarshal_func,
		   };

1;
