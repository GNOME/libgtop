/* $Id$ */

/* Copyright (C) 1995, 1996, 1997 Free Software Foundation, Inc.
   This file is part of the Gnome Top Library.
   Contributed by Martin Baulig <martin@home-of-linux.org>, April 1998.

   The Gnome Top Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Top Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include <glibtop/procdata.h>

const char *glibtop_names_procdata [GLIBTOP_MAX_PROCDATA] =
{ "cmd", "state", "uid", "pid", "ppid", "pgrp", "session",
  "tty", "tpgid", "priority", "nice", "signal", "blocked",
  "sigignore", "sigcatch", "start_time", "utime", "stime",
  "cutime", "cstime", "size", "resident", "share", "trs",
  "lrs", "drs", "dt", "vsize", "rss", "rss_rlim", "timeout",
  "it_real_value", "k_flags", "min_flt", "maj_flt", "cmin_flt",
  "cmaj_flt", "start_code", "end_code", "start_stack",
  "kstk_esp", "kstk_eip", "wchan"
};

const char *glibtop_labels_procdata [GLIBTOP_MAX_PROCDATA] =
{ N_("Cmd"), N_("Stat"), N_("UID"), N_("PID"), N_("PPID"), N_("PGRP"),
  N_("Session"), N_("Tty"), N_("TPGID"), N_("Priority"), N_("Nice"),
  N_("Signal"), N_("Blocked"), N_("SigIgnore"), N_("SigCatch"),
  N_("Start_Time"), N_("UTime"), N_("STime"), N_("CUTime"), N_("CSTime"),
  N_("Size"), N_("Resident"), N_("Share"), N_("TRS"), N_("LRS"), N_("DRS"),
  N_("DT"), N_("VSize"), N_("RSS"), N_("RSS_RLim"), N_("Timeout"),
  N_("It_Real_Value"), N_("Flags"), N_("Min_Flt"), N_("Maj_Flt"),
  N_("CMin_Flt"), N_("Cmaj_Flt"), N_("Start_Code"), N_("End_Code"),
  N_("Start_Stack"), N_("KSTK_ESP"), N_("KSTK_EIP"), N_("WChan")
};

const char *glibtop_descriptions_procdata [GLIBTOP_MAX_PROCDATA] =
{ NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
  NULL, NULL
};
