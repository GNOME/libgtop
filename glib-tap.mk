<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
  "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns='http://www.w3.org/1999/xhtml' xml:lang='en' lang='en'>
<head>
<title>glib - Low level core library</title>
<meta name='generator' content='cgit v0.9.2'/>
<meta name='robots' content='index, nofollow'/>
<link rel='stylesheet' type='text/css' href='/browse/cgit-gnome.css'/>
<link rel='shortcut icon' href='http://www-old.gnome.org/img/logo/foot-16.png'/>
<link rel='alternate' title='Atom feed' href='https://git.gnome.org/browse/glib/atom/glib-tap.mk?h=master' type='application/atom+xml'/>
</head>
<body>
<div id="global_domain_bar">
<div class="maxwidth">
	<div class="tab">
	<a class="root" href="https://www.gnome.org/">GNOME.org</a>
	</div>
</div>
</div>

<div id="page">
    <div id="logo_bar" class="container_12">
      <div id="logo" class="grid_3">
        <a title="Go to home page" href="https://git.gnome.org/"><img src="https://static.gnome.org/img/gnome-git.png" alt="GNOME: Git Repository" /></a>
      </div>

      <div id="top_bar" class="grid_9">
        <div class="left">
          <div class="menu-globalnav-container">
            <ul id="menu-globalnav" class="menu">
              <li id="menu-item-1039" class=
              "menu-item menu-item-type-post_type menu-item-object-page menu-item-1039">
              <a href="https://git.gnome.org/">Home</a></li>

              <li id="menu-item-1037" class=
              "menu-item menu-item-type-post_type menu-item-object-page menu-item-1037">
              <a href="https://wiki.gnome.org/Git">Git Help</a></li>
            </ul>
          </div>
        </div>
      </div>
      
    </div>
</div>
<div id='cgit'><table id='header'>
<tr>
<td class='main'><a href='/browse/'>index</a> : <a title='glib' href='/browse/glib/'>glib</a></td><td class='form'><form method='get' action=''>
<select name='h' onchange='this.form.submit();'>
<option value='GLIB_1_1_3_MARTIN'>GLIB_1_1_3_MARTIN</option>
<option value='GLIB_1_1_4_THREADS'>GLIB_1_1_4_THREADS</option>
<option value='GLIB_1_3_HACKS'>GLIB_1_3_HACKS</option>
<option value='GLIB_2_15_0'>GLIB_2_15_0</option>
<option value='dispatch-data'>dispatch-data</option>
<option value='gdbus-daemon'>gdbus-daemon</option>
<option value='gdbus-daemon2'>gdbus-daemon2</option>
<option value='glib-1-2'>glib-1-2</option>
<option value='glib-2-0'>glib-2-0</option>
<option value='glib-2-10'>glib-2-10</option>
<option value='glib-2-12'>glib-2-12</option>
<option value='glib-2-14'>glib-2-14</option>
<option value='glib-2-16'>glib-2-16</option>
<option value='glib-2-18'>glib-2-18</option>
<option value='glib-2-2'>glib-2-2</option>
<option value='glib-2-20'>glib-2-20</option>
<option value='glib-2-22'>glib-2-22</option>
<option value='glib-2-24'>glib-2-24</option>
<option value='glib-2-26'>glib-2-26</option>
<option value='glib-2-28'>glib-2-28</option>
<option value='glib-2-30'>glib-2-30</option>
<option value='glib-2-32'>glib-2-32</option>
<option value='glib-2-34'>glib-2-34</option>
<option value='glib-2-36'>glib-2-36</option>
<option value='glib-2-38'>glib-2-38</option>
<option value='glib-2-4'>glib-2-4</option>
<option value='glib-2-6'>glib-2-6</option>
<option value='glib-2-8'>glib-2-8</option>
<option value='glib-main-loop'>glib-main-loop</option>
<option value='glib-threads'>glib-threads</option>
<option value='master' selected='selected'>master</option>
<option value='new-gsettings'>new-gsettings</option>
<option value='signal-performance'>signal-performance</option>
<option value='test/gobjectnew'>test/gobjectnew</option>
<option value='tizen/kdbus-dev'>tizen/kdbus-dev</option>
<option value='wip/async-io-perf'>wip/async-io-perf</option>
<option value='wip/child-catchall'>wip/child-catchall</option>
<option value='wip/coverity-fixes'>wip/coverity-fixes</option>
<option value='wip/danw/clicert'>wip/danw/clicert</option>
<option value='wip/danw/glib.mk'>wip/danw/glib.mk</option>
<option value='wip/dbusable'>wip/dbusable</option>
<option value='wip/desktop-actions'>wip/desktop-actions</option>
<option value='wip/doc-fixes'>wip/doc-fixes</option>
<option value='wip/format-errors'>wip/format-errors</option>
<option value='wip/free'>wip/free</option>
<option value='wip/g-action-print-detailed-name'>wip/g-action-print-detailed-name</option>
<option value='wip/gbytes-takeover'>wip/gbytes-takeover</option>
<option value='wip/gcleanup'>wip/gcleanup</option>
<option value='wip/gcleanup-desrt'>wip/gcleanup-desrt</option>
<option value='wip/gdesktopappinfo'>wip/gdesktopappinfo</option>
<option value='wip/gicon'>wip/gicon</option>
<option value='wip/gnotification'>wip/gnotification</option>
<option value='wip/gobjectnew'>wip/gobjectnew</option>
<option value='wip/gproperty'>wip/gproperty</option>
<option value='wip/gproperty-2'>wip/gproperty-2</option>
<option value='wip/gsettings-list'>wip/gsettings-list</option>
<option value='wip/gsettings-work'>wip/gsettings-work</option>
<option value='wip/gsubprocess'>wip/gsubprocess</option>
<option value='wip/gsubprocess-2.36'>wip/gsubprocess-2.36</option>
<option value='wip/gsubprocess-desrt'>wip/gsubprocess-desrt</option>
<option value='wip/gsubprocess-ostreams'>wip/gsubprocess-ostreams</option>
<option value='wip/gutils-splitup'>wip/gutils-splitup</option>
<option value='wip/installed-tests'>wip/installed-tests</option>
<option value='wip/latest-gsubprocess'>wip/latest-gsubprocess</option>
<option value='wip/le-gsubprocess'>wip/le-gsubprocess</option>
<option value='wip/linux'>wip/linux</option>
<option value='wip/locale-monitor'>wip/locale-monitor</option>
<option value='wip/makefile.glib'>wip/makefile.glib</option>
<option value='wip/mapped-bytes'>wip/mapped-bytes</option>
<option value='wip/new-parser'>wip/new-parser</option>
<option value='wip/pcre-jit'>wip/pcre-jit</option>
<option value='wip/pcre-mark'>wip/pcre-mark</option>
<option value='wip/private-rework-3'>wip/private-rework-3</option>
<option value='wip/resources2'>wip/resources2</option>
<option value='wip/serializable'>wip/serializable</option>
<option value='wip/settings-backend'>wip/settings-backend</option>
<option value='wip/subprocess-2013'>wip/subprocess-2013</option>
<option value='wip/symbol-visibility'>wip/symbol-visibility</option>
<option value='wip/task'>wip/task</option>
<option value='wip/test-cleanup'>wip/test-cleanup</option>
<option value='wip/threadsafe-qdata'>wip/threadsafe-qdata</option>
<option value='wip/unicode-graphemebreak'>wip/unicode-graphemebreak</option>
<option value='wip/version-bounds'>wip/version-bounds</option>
<option value='wip/win32-source-api'>wip/win32-source-api</option>
</select> <input type='submit' name='' value='switch'/></form></td></tr>
<tr><td class='sub'>Low level core library</td><td class='sub right'></td></tr></table>
<table class='tabs'><tr><td>
<a href='/browse/glib/'>summary</a><a href='/browse/glib/refs/'>refs</a><a href='/browse/glib/log/glib-tap.mk'>log</a><a class='active' href='/browse/glib/tree/glib-tap.mk'>tree</a><a href='/browse/glib/commit/glib-tap.mk'>commit</a><a href='/browse/glib/diff/glib-tap.mk'>diff</a></td><td class='form'><form class='right' method='get' action='/browse/glib/log/glib-tap.mk'>
<select name='qt'>
<option value='grep'>log msg</option>
<option value='author'>author</option>
<option value='committer'>committer</option>
<option value='range'>range</option>
</select>
<input class='txt' type='text' size='10' name='q' value=''/>
<input type='submit' value='search'/>
</form>
</td></tr></table>
<div class='path'>path: <a href='/browse/glib/tree/'>root</a>/<a href='/browse/glib/tree/glib-tap.mk'>glib-tap.mk</a></div><div class='content'>blob: c97bf3f2813a580862c7f73d7dc62b7260a848a9 (<a href='/browse/glib/plain/glib-tap.mk'>plain</a>)
<table summary='blob content' class='blob'>
<tr><td class='linenumbers'><pre><a class='no' id='n1' name='n1' href='#n1'>1</a>
<a class='no' id='n2' name='n2' href='#n2'>2</a>
<a class='no' id='n3' name='n3' href='#n3'>3</a>
<a class='no' id='n4' name='n4' href='#n4'>4</a>
<a class='no' id='n5' name='n5' href='#n5'>5</a>
<a class='no' id='n6' name='n6' href='#n6'>6</a>
<a class='no' id='n7' name='n7' href='#n7'>7</a>
<a class='no' id='n8' name='n8' href='#n8'>8</a>
<a class='no' id='n9' name='n9' href='#n9'>9</a>
<a class='no' id='n10' name='n10' href='#n10'>10</a>
<a class='no' id='n11' name='n11' href='#n11'>11</a>
<a class='no' id='n12' name='n12' href='#n12'>12</a>
<a class='no' id='n13' name='n13' href='#n13'>13</a>
<a class='no' id='n14' name='n14' href='#n14'>14</a>
<a class='no' id='n15' name='n15' href='#n15'>15</a>
<a class='no' id='n16' name='n16' href='#n16'>16</a>
<a class='no' id='n17' name='n17' href='#n17'>17</a>
<a class='no' id='n18' name='n18' href='#n18'>18</a>
<a class='no' id='n19' name='n19' href='#n19'>19</a>
<a class='no' id='n20' name='n20' href='#n20'>20</a>
<a class='no' id='n21' name='n21' href='#n21'>21</a>
<a class='no' id='n22' name='n22' href='#n22'>22</a>
<a class='no' id='n23' name='n23' href='#n23'>23</a>
<a class='no' id='n24' name='n24' href='#n24'>24</a>
<a class='no' id='n25' name='n25' href='#n25'>25</a>
<a class='no' id='n26' name='n26' href='#n26'>26</a>
<a class='no' id='n27' name='n27' href='#n27'>27</a>
<a class='no' id='n28' name='n28' href='#n28'>28</a>
<a class='no' id='n29' name='n29' href='#n29'>29</a>
<a class='no' id='n30' name='n30' href='#n30'>30</a>
<a class='no' id='n31' name='n31' href='#n31'>31</a>
<a class='no' id='n32' name='n32' href='#n32'>32</a>
<a class='no' id='n33' name='n33' href='#n33'>33</a>
<a class='no' id='n34' name='n34' href='#n34'>34</a>
<a class='no' id='n35' name='n35' href='#n35'>35</a>
<a class='no' id='n36' name='n36' href='#n36'>36</a>
<a class='no' id='n37' name='n37' href='#n37'>37</a>
<a class='no' id='n38' name='n38' href='#n38'>38</a>
<a class='no' id='n39' name='n39' href='#n39'>39</a>
<a class='no' id='n40' name='n40' href='#n40'>40</a>
<a class='no' id='n41' name='n41' href='#n41'>41</a>
<a class='no' id='n42' name='n42' href='#n42'>42</a>
<a class='no' id='n43' name='n43' href='#n43'>43</a>
<a class='no' id='n44' name='n44' href='#n44'>44</a>
<a class='no' id='n45' name='n45' href='#n45'>45</a>
<a class='no' id='n46' name='n46' href='#n46'>46</a>
<a class='no' id='n47' name='n47' href='#n47'>47</a>
<a class='no' id='n48' name='n48' href='#n48'>48</a>
<a class='no' id='n49' name='n49' href='#n49'>49</a>
<a class='no' id='n50' name='n50' href='#n50'>50</a>
<a class='no' id='n51' name='n51' href='#n51'>51</a>
<a class='no' id='n52' name='n52' href='#n52'>52</a>
<a class='no' id='n53' name='n53' href='#n53'>53</a>
<a class='no' id='n54' name='n54' href='#n54'>54</a>
<a class='no' id='n55' name='n55' href='#n55'>55</a>
<a class='no' id='n56' name='n56' href='#n56'>56</a>
<a class='no' id='n57' name='n57' href='#n57'>57</a>
<a class='no' id='n58' name='n58' href='#n58'>58</a>
<a class='no' id='n59' name='n59' href='#n59'>59</a>
<a class='no' id='n60' name='n60' href='#n60'>60</a>
<a class='no' id='n61' name='n61' href='#n61'>61</a>
<a class='no' id='n62' name='n62' href='#n62'>62</a>
<a class='no' id='n63' name='n63' href='#n63'>63</a>
<a class='no' id='n64' name='n64' href='#n64'>64</a>
<a class='no' id='n65' name='n65' href='#n65'>65</a>
<a class='no' id='n66' name='n66' href='#n66'>66</a>
<a class='no' id='n67' name='n67' href='#n67'>67</a>
<a class='no' id='n68' name='n68' href='#n68'>68</a>
<a class='no' id='n69' name='n69' href='#n69'>69</a>
<a class='no' id='n70' name='n70' href='#n70'>70</a>
<a class='no' id='n71' name='n71' href='#n71'>71</a>
<a class='no' id='n72' name='n72' href='#n72'>72</a>
<a class='no' id='n73' name='n73' href='#n73'>73</a>
<a class='no' id='n74' name='n74' href='#n74'>74</a>
<a class='no' id='n75' name='n75' href='#n75'>75</a>
<a class='no' id='n76' name='n76' href='#n76'>76</a>
<a class='no' id='n77' name='n77' href='#n77'>77</a>
<a class='no' id='n78' name='n78' href='#n78'>78</a>
<a class='no' id='n79' name='n79' href='#n79'>79</a>
<a class='no' id='n80' name='n80' href='#n80'>80</a>
<a class='no' id='n81' name='n81' href='#n81'>81</a>
<a class='no' id='n82' name='n82' href='#n82'>82</a>
<a class='no' id='n83' name='n83' href='#n83'>83</a>
<a class='no' id='n84' name='n84' href='#n84'>84</a>
<a class='no' id='n85' name='n85' href='#n85'>85</a>
<a class='no' id='n86' name='n86' href='#n86'>86</a>
<a class='no' id='n87' name='n87' href='#n87'>87</a>
<a class='no' id='n88' name='n88' href='#n88'>88</a>
<a class='no' id='n89' name='n89' href='#n89'>89</a>
<a class='no' id='n90' name='n90' href='#n90'>90</a>
<a class='no' id='n91' name='n91' href='#n91'>91</a>
<a class='no' id='n92' name='n92' href='#n92'>92</a>
<a class='no' id='n93' name='n93' href='#n93'>93</a>
<a class='no' id='n94' name='n94' href='#n94'>94</a>
<a class='no' id='n95' name='n95' href='#n95'>95</a>
<a class='no' id='n96' name='n96' href='#n96'>96</a>
<a class='no' id='n97' name='n97' href='#n97'>97</a>
<a class='no' id='n98' name='n98' href='#n98'>98</a>
<a class='no' id='n99' name='n99' href='#n99'>99</a>
<a class='no' id='n100' name='n100' href='#n100'>100</a>
<a class='no' id='n101' name='n101' href='#n101'>101</a>
<a class='no' id='n102' name='n102' href='#n102'>102</a>
<a class='no' id='n103' name='n103' href='#n103'>103</a>
<a class='no' id='n104' name='n104' href='#n104'>104</a>
<a class='no' id='n105' name='n105' href='#n105'>105</a>
<a class='no' id='n106' name='n106' href='#n106'>106</a>
<a class='no' id='n107' name='n107' href='#n107'>107</a>
<a class='no' id='n108' name='n108' href='#n108'>108</a>
<a class='no' id='n109' name='n109' href='#n109'>109</a>
<a class='no' id='n110' name='n110' href='#n110'>110</a>
<a class='no' id='n111' name='n111' href='#n111'>111</a>
<a class='no' id='n112' name='n112' href='#n112'>112</a>
<a class='no' id='n113' name='n113' href='#n113'>113</a>
<a class='no' id='n114' name='n114' href='#n114'>114</a>
<a class='no' id='n115' name='n115' href='#n115'>115</a>
<a class='no' id='n116' name='n116' href='#n116'>116</a>
<a class='no' id='n117' name='n117' href='#n117'>117</a>
<a class='no' id='n118' name='n118' href='#n118'>118</a>
<a class='no' id='n119' name='n119' href='#n119'>119</a>
<a class='no' id='n120' name='n120' href='#n120'>120</a>
<a class='no' id='n121' name='n121' href='#n121'>121</a>
<a class='no' id='n122' name='n122' href='#n122'>122</a>
<a class='no' id='n123' name='n123' href='#n123'>123</a>
<a class='no' id='n124' name='n124' href='#n124'>124</a>
<a class='no' id='n125' name='n125' href='#n125'>125</a>
<a class='no' id='n126' name='n126' href='#n126'>126</a>
<a class='no' id='n127' name='n127' href='#n127'>127</a>
<a class='no' id='n128' name='n128' href='#n128'>128</a>
<a class='no' id='n129' name='n129' href='#n129'>129</a>
<a class='no' id='n130' name='n130' href='#n130'>130</a>
<a class='no' id='n131' name='n131' href='#n131'>131</a>
<a class='no' id='n132' name='n132' href='#n132'>132</a>
<a class='no' id='n133' name='n133' href='#n133'>133</a>
<a class='no' id='n134' name='n134' href='#n134'>134</a>
</pre></td>
<td class='lines'><pre><code><span class="hl slc"># GLIB - Library of useful C routines</span>

TESTS_ENVIRONMENT<span class="hl opt">=</span> \
	G_TEST_SRCDIR<span class="hl opt">=</span><span class="hl str">&quot;$(abs_srcdir)&quot;</span> 		\
	G_TEST_BUILDDIR<span class="hl opt">=</span><span class="hl str">&quot;$(abs_builddir)&quot;</span> 	\
	G_DEBUG<span class="hl opt">=</span>gc-friendly 			\
	MALLOC_CHECK_<span class="hl opt">=</span><span class="hl num">2</span> 			\
	MALLOC_PERTURB_<span class="hl opt">=$$(($${</span>RANDOM<span class="hl opt">:-</span><span class="hl num">256</span><span class="hl opt">} %</span> <span class="hl num">256</span><span class="hl opt">))</span>
LOG_DRIVER <span class="hl opt">=</span> env AM_TAP_AWK<span class="hl opt">=</span><span class="hl str">'$(AWK)'</span> <span class="hl opt">$(</span>SHELL<span class="hl opt">) $(</span>top_srcdir<span class="hl opt">)/</span>tap-driver.sh
LOG_COMPILER <span class="hl opt">= $(</span>top_srcdir<span class="hl opt">)/</span>tap-test

NULL <span class="hl opt">=</span>

<span class="hl slc"># initialize variables for unconditional += appending</span>
BUILT_SOURCES <span class="hl opt">=</span>
BUILT_EXTRA_DIST <span class="hl opt">=</span>
CLEANFILES <span class="hl opt">= *</span>.log <span class="hl opt">*</span>.trs
DISTCLEANFILES <span class="hl opt">=</span>
MAINTAINERCLEANFILES <span class="hl opt">=</span>
EXTRA_DIST <span class="hl opt">=</span>
TESTS <span class="hl opt">=</span>

installed_test_LTLIBRARIES <span class="hl opt">=</span>
installed_test_PROGRAMS <span class="hl opt">=</span>
installed_test_SCRIPTS <span class="hl opt">=</span>
nobase_installed_test_DATA <span class="hl opt">=</span>

noinst_LTLIBRARIES <span class="hl opt">=</span>
noinst_PROGRAMS <span class="hl opt">=</span>
noinst_SCRIPTS <span class="hl opt">=</span>
noinst_DATA <span class="hl opt">=</span>

check_LTLIBRARIES <span class="hl opt">=</span>
check_PROGRAMS <span class="hl opt">=</span>
check_SCRIPTS <span class="hl opt">=</span>
check_DATA <span class="hl opt">=</span>

<span class="hl slc"># We support a fairly large range of possible variables.  It is expected that all types of files in a test suite</span>
<span class="hl slc"># will belong in exactly one of the following variables.</span>
<span class="hl slc">#</span>
<span class="hl slc"># First, we support the usual automake suffixes, but in lowercase, with the customary meaning:</span>
<span class="hl slc">#</span>
<span class="hl slc">#   test_programs, test_scripts, test_data, test_ltlibraries</span>
<span class="hl slc">#</span>
<span class="hl slc"># The above are used to list files that are involved in both uninstalled and installed testing.  The</span>
<span class="hl slc"># test_programs and test_scripts are taken to be actual testcases and will be run as part of the test suite.</span>
<span class="hl slc"># Note that _data is always used with the nobase_ automake variable name to ensure that installed test data is</span>
<span class="hl slc"># installed in the same way as it appears in the package layout.</span>
<span class="hl slc">#</span>
<span class="hl slc"># In order to mark a particular file as being only for one type of testing, use 'installed' or 'uninstalled',</span>
<span class="hl slc"># like so:</span>
<span class="hl slc">#</span>
<span class="hl slc">#   installed_test_programs, uninstalled_test_programs</span>
<span class="hl slc">#   installed_test_scripts, uninstalled_test_scripts</span>
<span class="hl slc">#   installed_test_data, uninstalled_test_data</span>
<span class="hl slc">#   installed_test_ltlibraries, uninstalled_test_ltlibraries</span>
<span class="hl slc">#</span>
<span class="hl slc"># Additionally, we support 'extra' infixes for programs and scripts.  This is used for support programs/scripts</span>
<span class="hl slc"># that should not themselves be run as testcases (but exist to be used from other testcases):</span>
<span class="hl slc">#</span>
<span class="hl slc">#   test_extra_programs, installed_test_extra_programs, uninstalled_test_extra_programs</span>
<span class="hl slc">#   test_extra_scripts, installed_test_extra_scripts, uninstalled_test_extra_scripts</span>
<span class="hl slc">#</span>
<span class="hl slc"># Additionally, for _scripts and _data, we support the customary dist_ prefix so that the named script or data</span>
<span class="hl slc"># file automatically end up in the tarball.</span>
<span class="hl slc">#</span>
<span class="hl slc">#   dist_test_scripts, dist_test_data, dist_test_extra_scripts</span>
<span class="hl slc">#   dist_installed_test_scripts, dist_installed_test_data, dist_installed_test_extra_scripts</span>
<span class="hl slc">#   dist_uninstalled_test_scripts, dist_uninstalled_test_data, dist_uninstalled_test_extra_scripts</span>
<span class="hl slc">#</span>
<span class="hl slc"># Note that no file is automatically disted unless it appears in one of the dist_ variables.  This follows the</span>
<span class="hl slc"># standard automake convention of not disting programs scripts or data by default.</span>
<span class="hl slc">#</span>
<span class="hl slc"># test_programs, test_scripts, uninstalled_test_programs and uninstalled_test_scripts (as well as their disted</span>
<span class="hl slc"># variants) will be run as part of the in-tree 'make check'.  These are all assumed to be runnable under</span>
<span class="hl slc"># gtester.  That's a bit strange for scripts, but it's possible.</span>

TESTS <span class="hl opt">+= $(</span>test_programs<span class="hl opt">) $(</span>test_scripts<span class="hl opt">) $(</span>uninstalled_test_programs<span class="hl opt">) $(</span>uninstalled_test_scripts<span class="hl opt">)</span> \
         <span class="hl opt">$(</span>dist_test_scripts<span class="hl opt">) $(</span>dist_uninstalled_test_scripts<span class="hl opt">)</span>

<span class="hl slc"># Note: build even the installed-only targets during 'make check' to ensure that they still work.</span>
<span class="hl slc"># We need to do a bit of trickery here and manage disting via EXTRA_DIST instead of using dist_ prefixes to</span>
<span class="hl slc"># prevent automake from mistreating gmake functions like $(wildcard ...) and $(addprefix ...) as if they were</span>
<span class="hl slc"># filenames, including removing duplicate instances of the opening part before the space, eg. '$(addprefix'.</span>
all_test_programs     <span class="hl opt">= $(</span>test_programs<span class="hl opt">) $(</span>uninstalled_test_programs<span class="hl opt">) $(</span>installed_test_programs<span class="hl opt">)</span> \
                        <span class="hl opt">$(</span>test_extra_programs<span class="hl opt">) $(</span>uninstalled_test_extra_programs<span class="hl opt">) $(</span>installed_test_extra_programs<span class="hl opt">)</span>
all_test_scripts      <span class="hl opt">= $(</span>test_scripts<span class="hl opt">) $(</span>uninstalled_test_scripts<span class="hl opt">) $(</span>installed_test_scripts<span class="hl opt">)</span> \
                        <span class="hl opt">$(</span>test_extra_scripts<span class="hl opt">) $(</span>uninstalled_test_extra_scripts<span class="hl opt">) $(</span>installed_test_extra_scripts<span class="hl opt">)</span>
all_dist_test_scripts <span class="hl opt">= $(</span>dist_test_scripts<span class="hl opt">) $(</span>dist_uninstalled_test_scripts<span class="hl opt">) $(</span>dist_installed_test_scripts<span class="hl opt">)</span> \
                        <span class="hl opt">$(</span>dist_test_extra_scripts<span class="hl opt">) $(</span>dist_uninstalled_test_extra_scripts<span class="hl opt">) $(</span>dist_installed_test_extra_scripts<span class="hl opt">)</span>
all_test_scripts     <span class="hl opt">+= $(</span>all_dist_test_scripts<span class="hl opt">)</span>
EXTRA_DIST           <span class="hl opt">+= $(</span>all_dist_test_scripts<span class="hl opt">)</span>
all_test_data         <span class="hl opt">= $(</span>test_data<span class="hl opt">) $(</span>uninstalled_test_data<span class="hl opt">) $(</span>installed_test_data<span class="hl opt">)</span>
all_dist_test_data    <span class="hl opt">= $(</span>dist_test_data<span class="hl opt">) $(</span>dist_uninstalled_test_data<span class="hl opt">) $(</span>dist_installed_test_data<span class="hl opt">)</span>
all_test_data        <span class="hl opt">+= $(</span>all_dist_test_data<span class="hl opt">)</span>
EXTRA_DIST           <span class="hl opt">+= $(</span>all_dist_test_data<span class="hl opt">)</span>
all_test_ltlibs       <span class="hl opt">= $(</span>test_ltlibraries<span class="hl opt">) $(</span>uninstalled_test_ltlibraries<span class="hl opt">) $(</span>installed_test_ltlibraries<span class="hl opt">)</span>

if ENABLE_ALWAYS_BUILD_TESTS
noinst_LTLIBRARIES <span class="hl opt">+= $(</span>all_test_ltlibs<span class="hl opt">)</span>
noinst_PROGRAMS <span class="hl opt">+= $(</span>all_test_programs<span class="hl opt">)</span>
noinst_SCRIPTS <span class="hl opt">+= $(</span>all_test_scripts<span class="hl opt">)</span>
noinst_DATA <span class="hl opt">+= $(</span>all_test_data<span class="hl opt">)</span>
<span class="hl kwb">else</span>
check_LTLIBRARIES <span class="hl opt">+= $(</span>all_test_ltlibs<span class="hl opt">)</span>
check_PROGRAMS <span class="hl opt">+= $(</span>all_test_programs<span class="hl opt">)</span>
check_SCRIPTS <span class="hl opt">+= $(</span>all_test_scripts<span class="hl opt">)</span>
check_DATA <span class="hl opt">+= $(</span>all_test_data<span class="hl opt">)</span>
<span class="hl kwb">endif</span>

if ENABLE_INSTALLED_TESTS
installed_test_PROGRAMS <span class="hl opt">+= $(</span>test_programs<span class="hl opt">) $(</span>installed_test_programs<span class="hl opt">)</span> \
                          <span class="hl opt">$(</span>test_extra_programs<span class="hl opt">) $(</span>installed_test_extra_programs<span class="hl opt">)</span>
installed_test_SCRIPTS <span class="hl opt">+= $(</span>test_scripts<span class="hl opt">) $(</span>installed_test_scripts<span class="hl opt">)</span> \
                          <span class="hl opt">$(</span>test_extra_scripts<span class="hl opt">) $(</span>test_installed_extra_scripts<span class="hl opt">)</span>
installed_test_SCRIPTS <span class="hl opt">+= $(</span>dist_test_scripts<span class="hl opt">) $(</span>dist_test_extra_scripts<span class="hl opt">)</span> \
                          <span class="hl opt">$(</span>dist_installed_test_scripts<span class="hl opt">) $(</span>dist_installed_test_extra_scripts<span class="hl opt">)</span>
nobase_installed_test_DATA <span class="hl opt">+= $(</span>test_data<span class="hl opt">) $(</span>installed_test_data<span class="hl opt">)</span>
nobase_installed_test_DATA <span class="hl opt">+= $(</span>dist_test_data<span class="hl opt">) $(</span>dist_installed_test_data<span class="hl opt">)</span>
installed_test_LTLIBRARIES <span class="hl opt">+= $(</span>test_ltlibraries<span class="hl opt">) $(</span>installed_test_ltlibraries<span class="hl opt">)</span>
installed_testcases <span class="hl opt">= $(</span>test_programs<span class="hl opt">) $(</span>installed_test_programs<span class="hl opt">)</span> \
                      <span class="hl opt">$(</span>test_scripts<span class="hl opt">) $(</span>installed_test_scripts<span class="hl opt">)</span> \
                      <span class="hl opt">$(</span>dist_test_scripts<span class="hl opt">) $(</span>dist_installed_test_scripts<span class="hl opt">)</span>

installed_test_meta_DATA <span class="hl opt">= $(</span>installed_testcases<span class="hl opt">:=</span>.<span class="hl kwa">test</span><span class="hl opt">)</span>

<span class="hl opt">%</span>.<span class="hl kwa">test</span><span class="hl opt">: %$(</span>EXEEXT<span class="hl opt">)</span> Makefile
	<span class="hl opt">$(</span>AM_V_GEN<span class="hl opt">) (</span>echo <span class="hl str">'[Test]'</span> <span class="hl opt">&gt; $</span>&#64;.tmp<span class="hl opt">;</span> \
	echo <span class="hl str">'Type=session'</span> <span class="hl opt">&gt;&gt; $</span>&#64;.tmp<span class="hl opt">;</span> \
	echo <span class="hl str">'Exec=$(installed_testdir)/$&lt;'</span> <span class="hl opt">&gt;&gt; $</span>&#64;.tmp<span class="hl opt">;</span> \
	mv <span class="hl opt">$</span>&#64;.tmp <span class="hl opt">$</span>&#64;<span class="hl opt">)</span>

CLEANFILES <span class="hl opt">+= $(</span>installed_test_meta_DATA<span class="hl opt">)</span>
<span class="hl kwb">endif</span>
</code></pre></td></tr></table>
</div> <!-- class=content -->
</div>
  <div id="footer_community"></div>

  <div id="footer_grass"></div>

  <div id="footer">
    <div class="container_13" id="container_12">
      <div class="links grid_9">
        <div class="menu-footer-container">
          <ul id="menu-footer" class="menu">
            <li id="menu-item-1048" class=
            "menu-item menu-item-type-custom menu-item-object-custom current-menu-item current_page_item menu-item-1048">
            <a href="/">The GNOME Project</a>

              <ul class="sub-menu">
                <li id="menu-item-1049" class=
                "menu-item menu-item-type-post_type menu-item-object-page menu-item-1049">
                <a href="https://www.gnome.org/about/">About Us</a></li>

                <li id="menu-item-1050" class=
                "menu-item menu-item-type-post_type menu-item-object-page menu-item-1050">
                <a href="https://www.gnome.org/get-involved/">Get Involved</a></li>

                <li id="menu-item-1051" class=
                "menu-item menu-item-type-post_type menu-item-object-page menu-item-1051">
                <a href="https://www.gnome.org/teams/">Teams</a></li>

                <li id="menu-item-1053" class=
                "menu-item menu-item-type-post_type menu-item-object-page menu-item-1053">
                <a href="https://www.gnome.org/support-gnome/">Support GNOME</a></li>

                <li id="menu-item-1054" class=
                "menu-item menu-item-type-post_type menu-item-object-page menu-item-1054">
                <a href="https://www.gnome.org/contact/">Contact Us</a></li>

                <li id="menu-item-2246" class=
                "menu-item menu-item-type-post_type menu-item-object-page menu-item-2246">
                <a href="https://www.gnome.org/foundation/">The GNOME Foundation</a></li>
              </ul>
            </li>

            <li id="menu-item-1047" class=
            "menu-item menu-item-type-custom menu-item-object-custom menu-item-1047">
              <a href="#">Resources</a>

              <ul class="sub-menu">
                <li id="menu-item-1055" class=
                "menu-item menu-item-type-custom menu-item-object-custom menu-item-1055">
                <a href="https://developer.gnome.org">Developer Center</a></li>

                <li id="menu-item-1056" class=
                "menu-item menu-item-type-custom menu-item-object-custom menu-item-1056">
                <a href="https://help.gnome.org">Documentation</a></li>

                <li id="menu-item-1057" class=
                "menu-item menu-item-type-custom menu-item-object-custom menu-item-1057">
                <a href="https://wiki.gnome.org">Wiki</a></li>

                <li id="menu-item-1058" class=
                "menu-item menu-item-type-custom menu-item-object-custom menu-item-1058">
                <a href="https://mail.gnome.org/mailman/listinfo">Mailing Lists</a></li>

                <li id="menu-item-1059" class=
                "menu-item menu-item-type-custom menu-item-object-custom menu-item-1059">
                <a href="https://wiki.gnome.org/GnomeIrcChannels">IRC Channels</a></li>

                <li id="menu-item-1060" class=
                "menu-item menu-item-type-custom menu-item-object-custom menu-item-1060">
                <a href="https://bugzilla.gnome.org/">Bug Tracker</a></li>

                <li id="menu-item-1061" class=
                "menu-item menu-item-type-custom menu-item-object-custom menu-item-1061">
                <a href="https://git.gnome.org/browse/">Development Code</a></li>

                <li id="menu-item-1062" class=
                "menu-item menu-item-type-custom menu-item-object-custom menu-item-1062">
                <a href="https://wiki.gnome.org/Jhbuild">Build Tool</a></li>
              </ul>
            </li>

            <li id="menu-item-1046" class=
            "menu-item menu-item-type-custom menu-item-object-custom menu-item-1046">
              <a href="/news">News</a>

              <ul class="sub-menu">
                <li id="menu-item-1063" class=
                "menu-item menu-item-type-post_type menu-item-object-page menu-item-1063">
                <a href="https://www.gnome.org/press/">Press Releases</a></li>

                <li id="menu-item-1064" class=
                "menu-item menu-item-type-custom menu-item-object-custom menu-item-1064">
                <a href="https://www.gnome.org/start/stable">Latest Release</a></li>

                <li id="menu-item-1065" class=
                "menu-item menu-item-type-custom menu-item-object-custom menu-item-1065">
                <a href="https://planet.gnome.org">Planet GNOME</a></li>

                <li id="menu-item-1067" class=
                "menu-item menu-item-type-custom menu-item-object-custom menu-item-1067">
                <a href="https://news.gnome.org">Development News</a></li>

                <li id="menu-item-1068" class=
                "menu-item menu-item-type-custom menu-item-object-custom menu-item-1068">
                <a href="https://identi.ca/gnome">Identi.ca</a></li>

                <li id="menu-item-1069" class=
                "menu-item menu-item-type-custom menu-item-object-custom menu-item-1069">
                <a href="https://twitter.com/gnome">Twitter</a></li>
              </ul>
            </li>
          </ul>
        </div>
      </div>

      <div id="footnotes" class="grid_9">
<p>Copyright &copy; 2004-2013, <a href="https://www.gnome.org/">The GNOME Project</a>.</p>
<br />
  <small><p>Hosted by <a href="http://www.redhat.com/">Red Hat</a>. 
   Powered by <a href="http://hjemli.net/git/cgit/">cgit</a>.</p>
  <p>To follow the commits, subscribe to <a href="http://mail.gnome.org/mailman/listinfo/commits-list">commits-list</a>. (can be limited to specific modules)</p></small>
</div>
    </div>
  </div>
</div> <!-- id=cgit -->
</body>
</html>
