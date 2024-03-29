/* Diffuse - A graphical tool for merging and comparing text files
 *
 * Copyright (C) 2006-2014 Derrick Moser <derrick_moser@yahoo.com>
 * Copyright (C) 2022      Markus Prasser
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 2 of the license, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  You may also obtain a copy of the GNU General Public License
 * from the Free Software Foundation by visiting their web site
 * (http://www.fsf.org/) or by writing to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "df_diffuse.h"
#include "df_globals.h"
#include "df_resources.h"
#include "df_utils.h"

#include <gtkmm/main.h>
#include <gtkmm/window.h>

#include <glibmm/convert.h>
#include <glibmm/miscutils.h>

#include <iostream>

namespace Df = Diffuse;

static std::vector<Glib::ustring> convert_args(int argc, char *argv[]);
static bool make_subdirs(std::string &p, const std::vector<std::string> &ss);

char *Df::COPYRIGHT = nullptr;

int main(int argc, char *argv[]) {
  Df::COPYRIGHT =
      _("Copyright © 2006-2014 Derrick Moser\nCopyright © 2022 Markus Prasser");

  Gtk::Main app{argc, argv};

  // Use the location of the program as a starting place to search for
  // supporting files such as icons or documentation
  const std::string app_path{argv[0]};
  const std::string bin_dir{Glib::path_get_dirname(app_path)};

/*
# platform test
def isWindows():
    return os.name == 'nt'

# translation location: '../share/locale/<LANG>/LC_MESSAGES/diffuse.mo'
# where '<LANG>' is the language key
lang = locale.getdefaultlocale()[0]
if isWindows():
    # gettext looks for the language using environment variables which
    # are normally not set on Windows so we try setting it for them
    for v in 'LANGUAGE', 'LC_ALL', 'LC_MESSAGES', 'LANG':
        if v in os.environ:
            lang = os.environ[v]
            # remove any additional languages, encodings, or modifications
            for v in ':.@':
                lang = lang.split(v)[0]
            break
    else:
        if lang is not None:
            os.environ['LANG'] = lang
    del v
    locale_dir = 'locale'
else:
    locale_dir = '../share/locale'
locale_dir = os.path.join(bin_dir, locale_dir)
gettext.bindtextdomain('diffuse', locale_dir)

gettext.textdomain('diffuse')
_ = gettext.gettext
 */

  // Process help options
  const auto args{convert_args(argc, argv)};

  if ((2 == argc) && (("-v" == args[1]) || ("--version" == args[1]))) {
    Df::printMessage(Glib::ustring::compose("%1 %2\n%3", Df::APP_NAME,
                                            Df::VERSION, Df::COPYRIGHT));
    return 0;
  }
  if ((2 == argc) &&
      (("-?" == args[1]) || ("-h" == args[1]) || ("--help" == args[1]))) {
    Df::printMessage(
        "Usage:\n"
        "    diffuse [ [OPTION...] [FILE...] ]...\n"
        "    diffuse ( -h | -? | --help | -v | --version )\n"
        "\n"
        "Diffuse is a graphical tool for merging and comparing text files.  "
        "Diffuse is\n"
        "able to compare an arbitrary number of files side-by-side and gives "
        "users the\n"
        "ability to manually adjust line matching and directly edit files.  "
        "Diffuse can\n"
        "also retrieve revisions of files from Bazaar, CVS, Darcs, Git, "
        "Mercurial,\n"
        "Monotone, RCS, Subversion, and SVK repositories for comparison and "
        "merging.\n"
        "\n"
        "Help Options:\n"
        "  ( -h | -? | --help )             Display this usage information\n"
        "  ( -v | --version )               Display version and copyright "
        "information\n"
        "\n"
        "Configuration Options:\n"
        "  --no-rcfile                      Do not read any resource files\n"
        "  --rcfile <file>                  Specify explicit resource file\n"
        "\n"
        "General Options:\n"
        "  ( -c | --commit ) <rev>          File revisions <rev-1> and <rev>\n"
        "  ( -D | --close-if-same )         Close all tabs with no "
        "differences\n"
        "  ( -e | --encoding ) <codec>      Use <codec> to read and write "
        "files\n"
        "  ( -L | --label ) <label>         Display <label> instead of the "
        "file name\n"
        "  ( -m | --modified )              Create a new tab for each modified "
        "file\n"
        "  ( -r | --revision ) <rev>        File revision <rev>\n"
        "  ( -s | --separate )              Create a new tab for each file\n"
        "  ( -t | --tab )                   Start a new tab\n"
        "  ( -V | --vcs ) <vcs-list>        Version control system search "
        "order\n"
        "  --line <line>                    Start with line <line> selected\n"
        "  --null-file                      Create a blank file comparison "
        "pane\n"
        "\n"
        "Display Options:\n"
        "  ( -b | --ignore-space-change )   Ignore changes to white space\n"
        "  ( -B | --ignore-blank-lines )    Ignore changes in blank lines\n"
        "  ( -E | --ignore-end-of-line )    Ignore end of line differences\n"
        "  ( -i | --ignore-case )           Ignore case differences\n"
        "  ( -w | --ignore-all-space )      Ignore white space differences");
    return 0;
  }

/*
import pygtk
pygtk.require('2.0')
import gtk

import difflib
import encodings
import glob
import gobject
import pango
import re
import stat
import string
import subprocess
import unicodedata
import webbrowser

if not hasattr(__builtins__, 'WindowsError'):
    # define 'WindowsError' so 'except' statements will work on all platforms
    WindowsError = IOError

# escape special glob characters
def globEscape(s):
    m = dict([ (c, u'[%s]' % (c, )) for c in u'[]?*' ])
    return u''.join([ m.get(c, c) for c in s ])
 */

  // Associate the icon with all windows
  // (this is not being done automatically with some older (Py)GTK versions
  Gtk::Window::set_default_icon_name("diffuse");

/*
class SyntaxParser:
    # given a string and an initial state, identify the final state and tokens
    def parse(self, state_name, s):
        lookup = self.transitions_lookup
        transitions, blocks, start = lookup[state_name], [], 0
        while start < len(s):
            for pattern, token_type, next_state in transitions:
                m = pattern.match(s, start)
                if m is not None:
                    end, state_name = m.span()[1], next_state
                    transitions = lookup[state_name]
                    break
            else:
                end, token_type = start + 1, self.default_token_type
            if len(blocks) > 0 and blocks[-1][2] == token_type:
                blocks[-1][1] = end
            else:
                blocks.append([start, end, token_type])
            start = end
        return state_name, blocks

# split string into lines based upon DOS, Mac, and Unix line endings
def splitlines(s):
    # split on new line characters
    temp, i, n = [], 0, len(s)
    while i < n:
        j = s.find('\n', i)
        if j < 0:
            temp.append(s[i:])
            break
        j += 1
        temp.append(s[i:j])
        i = j
    # split on carriage return characters
    ss = []
    for s in temp:
        i, n = 0, len(s)
        while i < n:
            j = s.find('\r', i)
            if j < 0:
                ss.append(s[i:])
                break
            j += 1
            if j < n and s[j] == '\n':
                j += 1
            ss.append(s[i:j])
            i = j
    return ss

# also recognise old Mac OS line endings
def readlines(fd):
    return strip_eols(splitlines(fd.read()))

class Resources:
    # keyboard action processing
    def setKeyBinding(self, ctx, s, v):
        action_tuple = (ctx, s)
        modifiers = 0
        key = None
        for token in v.split('+'):
            if token == 'Shift':
                modifiers |= gtk.gdk.SHIFT_MASK
            elif token == 'Ctrl':
                modifiers |= gtk.gdk.CONTROL_MASK
            elif token == 'Alt':
                modifiers |= gtk.gdk.MOD1_MASK
            elif len(token) == 0 or token[0] == '_':
                raise ValueError()
            else:
                if token[0].isdigit():
                    token = '_' + token
                if not hasattr(gtk.keysyms, token):
                    raise ValueError()
                key = getattr(gtk.keysyms, token)
        if key is None:
            raise ValueError()
        key_tuple = (ctx, (key, modifiers))

        # remove any existing binding
        if key_tuple in self.keybindings_lookup:
            self._removeKeyBinding(key_tuple)

        # ensure we have a set to hold this action
        if action_tuple not in self.keybindings:
            self.keybindings[action_tuple] = {}
        bindings = self.keybindings[action_tuple]

        # menu items can only have one binding
        if ctx == 'menu':
            for k in bindings.keys():
                self._removeKeyBinding(k)

        # add the binding
        bindings[key_tuple] = None
        self.keybindings_lookup[key_tuple] = action_tuple

    def _removeKeyBinding(self, key_tuple):
        action_tuple = self.keybindings_lookup[key_tuple]
        del self.keybindings_lookup[key_tuple]
        del self.keybindings[action_tuple][key_tuple]

    def getActionForKey(self, ctx, key, modifiers):
        try:
            return self.keybindings_lookup[(ctx, (key, modifiers))][1]
        except KeyError:
            pass

    def getKeyBindings(self, ctx, s):
        try:
            return [ t for c, t in self.keybindings[(ctx, s)].keys() ]
        except KeyError:
            return []

    # syntax highlighting
    def getSyntaxNames(self):
        return self.syntaxes.keys()

    def getSyntax(self, name):
        return self.syntaxes.get(name, None)

    def guessSyntaxForFile(self, name, ss):
        name = os.path.basename(name)
        for key, pattern in self.syntax_file_patterns.items():
            if pattern.search(name):
                return key
        # fallback to analysing the first line of the file
        if len(ss) > 0:
            s = ss[0]
            for key, pattern in self.syntax_magic_patterns.items():
                if pattern.search(s):
                    return key

    def parse(self, file_name):
        for i, s in enumerate(ss):
            try:
                if args[0] == u'import' and len(args) == 2:
                    path = os.path.expanduser(args[1])
                    # relative paths are relative to the parsed file
                    path = os.path.join(globEscape(os.path.dirname(file_name)), path)
                    paths = glob.glob(path)
                    if len(paths) == 0:
                        paths = [ path ]
                    for path in paths:
                        # convert to absolute path so the location of
                        # any processing errors are reported with
                        # normalised file names
                        self.parse(os.path.abspath(path))
            except: # Grr... the 're' module throws weird errors
            #except ValueError:
                logError(_('Error processing line %(line)d of %(file)s.') % { 'line': i + 1, 'file': file_name })
 */

  Df::Resources theResources;

/*
# widget to help pick an encoding
class EncodingMenu(gtk.HBox):
    def __init__(self, prefs, autodetect=False):
        gtk.HBox.__init__(self)
        self.combobox = combobox = gtk.combo_box_new_text()
        self.encodings = prefs.getEncodings()[:]
        for e in self.encodings:
            combobox.append_text(e)
        if autodetect:
            self.encodings.insert(0, None)
            combobox.prepend_text(_('Auto Detect'))
        self.pack_start(combobox, False, False, 0)
        combobox.show()

    def set_text(self, encoding):
        encoding = norm_encoding(encoding)
        if encoding in self.encodings:
            self.combobox.set_active(self.encodings.index(encoding))

    def get_text(self):
        i = self.combobox.get_active()
        if i >= 0:
            return self.encodings[i]

class Preferences:
    # callback used when a preference is toggled
    def _toggled_cb(self, widget, widgets, name):
        # disable any preferences than are no longer relevant
        for k, v in self.disable_when.items():
            p, t = v
            if p == name:
                widgets[k].set_sensitive(widgets[p].get_active() != t)

    # display the dialogue and update the preference values if the accept
    # button was pressed
    def runDialog(self, parent):
        dialog = gtk.Dialog(_('Preferences'), parent, gtk.DIALOG_MODAL|gtk.DIALOG_DESTROY_WITH_PARENT, (gtk.STOCK_CANCEL, gtk.RESPONSE_REJECT, gtk.STOCK_OK, gtk.RESPONSE_OK))

        widgets = {}
        w = self._buildPrefsDialog(parent, widgets, self.template)
        # disable any preferences than are not relevant
        for k, v in self.disable_when.items():
            p, t = v
            if widgets[p].get_active() == t:
                widgets[k].set_sensitive(False)
        dialog.vbox.add(w)
        w.show()

        accept = (dialog.run() == gtk.RESPONSE_OK)
        if accept:
            for k in self.bool_prefs.keys():
                self.bool_prefs[k] = widgets[k].get_active()
            for k in self.int_prefs.keys():
                self.int_prefs[k] = widgets[k].get_value_as_int()
            for k in self.string_prefs.keys():
                self.string_prefs[k] = nullToEmpty(widgets[k].get_text())
            try:
                ss = []
                for k, v in self.bool_prefs.items():
                    if v != self.default_bool_prefs[k]:
                        ss.append('%s %s\n' % (k, v))
                for k, v in self.int_prefs.items():
                    if v != self.default_int_prefs[k]:
                        ss.append('%s %s\n' % (k, v))
                for k, v in self.string_prefs.items():
                    if v != self.default_string_prefs[k]:
                        ss.append('%s "%s"\n' % (k, v.replace('\\', '\\\\').replace('"', '\\"')))
                ss.sort()
                f = open(self.path, 'w')
                f.write('# This prefs file was generated by %s %s.\n\n' % (APP_NAME, VERSION))
                for s in ss:
                    f.write(s)
                f.close()
            except IOError:
                m = MessageDialog(parent, gtk.MESSAGE_ERROR, _('Error writing %s.') % (self.path, ))
                m.run()
                m.destroy()
        dialog.destroy()
        return accept

    # recursively traverses 'template' to build the preferences dialogue
    # and the individual preference widgets into 'widgets' so their value
    # can be easily queried by the caller
    def _buildPrefsDialog(self, parent, widgets, template):
        type = template[0]
        if type == 'FolderSet':
            notebook = gtk.Notebook()
            notebook.set_border_width(10)
            i = 1
            while i < len(template):
                label = gtk.Label(template[i])
                i += 1
                w = self._buildPrefsDialog(parent, widgets, template[i])
                i += 1
                notebook.append_page(w, label)
                w.show()
                label.show()
            return notebook
        else:
            n = len(template) - 1
            table = gtk.Table(2, n)
            table.set_border_width(10)
            for i, tpl in enumerate(template[1:]):
                type = tpl[0]
                if type == 'FolderSet':
                    w = self._buildPrefsDialog(parent, widgets, tpl)
                    table.attach(w, 0, 2, i, i + 1, gtk.FILL, gtk.FILL)
                    w.show()
                elif type == 'Boolean':
                    button = gtk.CheckButton(tpl[3])
                    button.set_active(self.bool_prefs[tpl[1]])
                    widgets[tpl[1]] = button
                    table.attach(button, 1, 2, i, i + 1, gtk.FILL, gtk.FILL)
                    button.connect('toggled', self._toggled_cb, widgets, tpl[1])
                    button.show()
                else:
                    label = gtk.Label(tpl[3] + ': ')
                    label.set_alignment(1.0, 0.5)
                    table.attach(label, 0, 1, i, i + 1, gtk.FILL, gtk.FILL)
                    label.show()
                    if tpl[0] in [ 'Font', 'Integer' ]:
                        entry = gtk.HBox()
                        if tpl[0] == 'Font':
                            button = FontButton()
                            button.set_font_name(self.string_prefs[tpl[1]])
                        else:
                            button = gtk.SpinButton(gtk.Adjustment(self.int_prefs[tpl[1]], tpl[4], tpl[5], 1), 0)
                        widgets[tpl[1]] = button
                        entry.pack_start(button, False, False, 0)
                        button.show()
                    else:
                        if tpl[0] == 'Encoding':
                            entry = EncodingMenu(self)
                            entry.set_text(tpl[3])
                        elif tpl[0] == 'File':
                            entry = FileEntry(parent, tpl[3])
                        else:
                            entry = gtk.Entry()
                        widgets[tpl[1]] = entry
                        entry.set_text(self.string_prefs[tpl[1]])
                    table.attach(entry, 1, 2, i, i + 1, gtk.EXPAND|gtk.FILL, gtk.FILL)
                    entry.show()
                table.show()
            return table

    def getEncodings(self):
        return self.encodings

    def _getDefaultEncodings(self):
        return self.string_prefs['encoding_auto_detect_codecs'].split()

    def getDefaultEncoding(self):
        return self.string_prefs['encoding_default_codec']

    # attempt to convert a string to unicode from an unknown encoding
    def convertToUnicode(self, s):
        # a BOM is required for autodetecting UTF16 and UTF32
        magic = { 'utf16': [ codecs.BOM_UTF16_BE, codecs.BOM_UTF16_LE ],
                  'utf32': [ codecs.BOM_UTF32_BE, codecs.BOM_UTF32_LE ] }
        for encoding in self._getDefaultEncodings():
            try:
                for m in magic.get(encoding.lower().replace('-', '').replace('_', ''), [ '' ]):
                    if s.startswith(m):
                        break
                else:
                    continue
                return unicode(s, encoding), encoding
            except (UnicodeDecodeError, LookupError):
                pass
        return u''.join([ unichr(ord(c)) for c in s ]), None

    # cygwin and native applications can be used on windows, use this method
    # to convert a path to the usual form expected on sys.platform
    def convertToNativePath(self, s):
        s = unicode(s, sys.getfilesystemencoding())
        if isWindows() and s.find('/') >= 0:
            # treat as a cygwin path
            s = s.replace(os.sep, '/')
            # convert to a Windows native style path
            p = [ a for a in s.split('/') if a != '' ]
            if s.startswith('//'):
                p[:0] = [ '', '' ]
            elif s.startswith('/'):
                pr = [ a for a in self.getString('cygwin_cygdrive_prefix').split('/') if a != '' ]
                n = len(pr)
                if len(p) > n and len(p[n]) == 1 and p[:n] == pr:
                    # path starts with cygdrive prefix
                    p[:n + 1] = [ p[n] + ':' ]
                else:
                    # full path
                    p[:0] = [ a for a in self.getString('cygwin_root').split(os.sep) if a != '' ]
            # add trailing slash
            if p[-1] != '' and s.endswith('/'):
                p.append('')
            s = os.sep.join(p)
        return s

# convenience method for creating a menu according to a template
def createMenu(specs, radio=None, accel_group=None):
    menu = gtk.Menu()
    for spec in specs:
        if len(spec) > 0:
            if len(spec) > 7 and spec[7] is not None:
                g, k = spec[7]
                if g not in radio:
                    item = gtk.RadioMenuItem(None, spec[0])
                    radio[g] = (item, {})
                else:
                    item = gtk.RadioMenuItem(radio[g][0], spec[0])
                radio[g][1][k] = item
            else:
                item = gtk.ImageMenuItem(spec[0])
            cb = spec[1]
            if cb is not None:
                data = spec[2]
                item.connect('activate', cb, data)
            if len(spec) > 3 and spec[3] is not None:
                image = gtk.Image()
                image.set_from_stock(spec[3], gtk.ICON_SIZE_MENU)
                item.set_image(image)
            if accel_group is not None and len(spec) > 4:
                a = theResources.getKeyBindings('menu', spec[4])
                if len(a) > 0:
                    key, modifier = a[0]
                    item.add_accelerator('activate', accel_group, key, modifier, gtk.ACCEL_VISIBLE)
            if len(spec) > 5:
                item.set_sensitive(spec[5])
            if len(spec) > 6 and spec[6] is not None:
                item.set_submenu(createMenu(spec[6], radio, accel_group))
        else:
            item = gtk.SeparatorMenuItem()
        menu.append(item)
        item.show()
    return menu

# convenience method for creating a menu bar according to a template
def createMenuBar(specs, radio, accel_group):
    menu_bar = gtk.MenuBar()
    for label, spec in specs:
        menu = gtk.MenuItem(label)
        menu.set_submenu(createMenu(spec, radio, accel_group))
        menu_bar.append(menu)
        menu.show()
    return menu_bar

# convenience method to set a widget's tooltip
def set_tooltip(widget, tip):
    if hasattr(widget, 'set_tooltip_text'):
        # only available in pygtk >= 2.12
        widget.set_tooltip_text(tip)
    else:
        # fallback on old tooltip method (pygtk < 2.12)
        widget.tooltip = gtk.Tooltips()
        widget.tooltip.set_tip(widget, tip)

# convenience method for packing buttons into a container according to a
# template
def appendButtons(box, size, specs):
    for spec in specs:
        if len(spec) > 0:
            button = gtk.Button()
            button.set_relief(gtk.RELIEF_NONE)
            button.unset_flags(gtk.CAN_FOCUS)
            image = gtk.Image()
            image.set_from_stock(spec[0], size)
            button.add(image)
            image.show()
            if len(spec) > 2:
                button.connect('clicked', spec[1], spec[2])
                if len(spec) > 3:
                    set_tooltip(button, spec[3])
            box.pack_start(button, False, False, 0)
            button.show()
        else:
            separator = gtk.VSeparator()
            box.pack_start(separator, False, False, 5)
            separator.show()

# masks used to indicate the presence of particular line endings
DOS_FORMAT = 1
MAC_FORMAT = 2
UNIX_FORMAT = 4

# True if the string ends with '\r\n'
def has_dos_line_ending(s):
    return s.endswith('\r\n')

# True if the string ends with '\r'
def has_mac_line_ending(s):
    return s.endswith('\r')

# True if the string ends with '\n' but not '\r\n'
def has_unix_line_ending(s):
    return s.endswith('\n') and not s.endswith('\r\n')

# returns the format mask for a list of strings
def getFormat(ss):
    flags = 0
    for s in ss:
        if s is not None:
            if has_dos_line_ending(s):
                flags |= DOS_FORMAT
            elif has_mac_line_ending(s):
                flags |= MAC_FORMAT
            elif has_unix_line_ending(s):
                flags |= UNIX_FORMAT
    return flags

# returns the number of characters in the string excluding any line ending
# characters
def len_minus_line_ending(s):
    if s is None:
        return 0
    n = len(s)
    if s.endswith('\r\n'):
        n -= 2
    elif s.endswith('\r') or s.endswith('\n'):
        n -= 1
    return n

# returns the string without the line ending characters
def strip_eol(s):
    if s is not None:
        return s[:len_minus_line_ending(s)]

# returns the list of strings without line ending characters
def strip_eols(ss):
    return [ strip_eol(s) for s in ss ]

# convenience method to change the line ending of a string
def convert_to_format(s, format):
    if s is not None and format != 0:
        old_format = getFormat([ s ])
        if old_format != 0 and (old_format & format) == 0:
            s = strip_eol(s)
            # prefer the host line ending style
            if (format & DOS_FORMAT) and os.linesep == '\r\n':
                s += os.linesep
            elif (format & MAC_FORMAT) and os.linesep == '\r':
                s += os.linesep
            elif (format & UNIX_FORMAT) and os.linesep == '\n':
                s += os.linesep
            elif format & UNIX_FORMAT:
                s += '\n'
            elif format & DOS_FORMAT:
                s += '\r\n'
            elif format & MAC_FORMAT:
                s += '\r'
    return s

# returns the Windows drive or share from a from an absolute path
def drive_from_path(s):
    c = s.split(os.sep)
    if len(c) > 3 and c[0] == '' and c[1] == '':
        return os.path.join(c[:4])
    return c[0]

# constructs a relative path from 'a' to 'b', both should be absolute paths
def relpath(a, b):
    if isWindows():
        if drive_from_path(a) != drive_from_path(b):
            return b
    c1 = [ c for c in a.split(os.sep) if c != '' ]
    c2 = [ c for c in b.split(os.sep) if c != '' ]
    i, n = 0, len(c1)
    while i < n and i < len(c2) and c1[i] == c2[i]:
        i += 1
    r = (n - i) * [ os.pardir ]
    r.extend(c2[i:])
    return os.sep.join(r)

# helper function prevent files from being confused with command line options
# by prepending './' to the basename
def safeRelativePath(abspath1, name, prefs, cygwin_pref):
    s = os.path.join(os.curdir, relpath(abspath1, os.path.abspath(name)))
    s = codecs.encode(s, sys.getfilesystemencoding())
    if isWindows():
        if prefs.getBool(cygwin_pref):
            s = s.replace('\\', '/')
        else:
            s = s.replace('/', '\\')
    return s

# escape arguments for use with bash
def bashEscape(s):
    return "'" + s.replace("'", "'\\''") + "'"

# use popen to read the output of a command
def popenRead(dn, cmd, prefs, bash_pref, success_results=None):
    if success_results is None:
        success_results = [ 0 ]
    if isWindows() and prefs.getBool(bash_pref):
        # launch the command from a bash shell is requested
        cmd = [ prefs.convertToNativePath('/bin/bash.exe'), '-l', '-c', 'cd %s; %s' % (bashEscape(dn), ' '.join([ bashEscape(arg) for arg in cmd ])) ]
        dn = None
    # use subprocess.Popen to retrieve the file contents
    if isWindows():
        info = subprocess.STARTUPINFO()
        info.dwFlags |= subprocess.STARTF_USESHOWWINDOW
        info.wShowWindow = subprocess.SW_HIDE
    else:
        info = None
    proc = subprocess.Popen(cmd, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, cwd=dn, startupinfo=info)
    proc.stdin.close()
    proc.stderr.close()
    fd = proc.stdout
    # read the command's output
    s = fd.read()
    fd.close()
    if proc.wait() not in success_results:
        raise IOError('Command failed.')
    return s

# use popen to read the output of a command
def popenReadLines(dn, cmd, prefs, bash_pref, success_results=None):
    return strip_eols(splitlines(popenRead(dn, cmd, prefs, bash_pref, success_results)))

# simulate use of popen with xargs to read the output of a command
def popenXArgsReadLines(dn, cmd, args, prefs, bash_pref):
    # os.sysconf() is only available on Unix
    if hasattr(os, 'sysconf'):
        maxsize = os.sysconf('SC_ARG_MAX')
        maxsize -= sum([ len(k) + len(v) + 2 for k, v in os.environ.items() ])
    else:
        # assume the Window's limit to CreateProcess()
        maxsize = 32767
    maxsize -= sum([ len(k) + 1 for k in cmd ])

    ss = []
    i, s, a = 0, 0, []
    while i < len(args):
        f = (len(a) == 0)
        if f:
            # start a new command line
            a = cmd[:]
        elif s + len(args[i]) + 1 <= maxsize:
            f = True
        if f:
            # append another argument to the current command line
            a.append(args[i])
            s += len(args[i]) + 1
            i += 1
        if i == len(args) or not f:
            ss.extend(popenReadLines(dn, a, prefs, bash_pref))
            s, a = 0, []
    return ss

# utility class to help support Git and Monotone
# represents a set of files and folders of interest for "git status" or
# "mtn automate inventory"
class _VcsFolderSet:
    def __init__(self, names):
        self.folders = f = []
        for name in names:
            name = os.path.abspath(name)
            # ensure all names end with os.sep
            if not name.endswith(os.sep):
                name += os.sep
            f.append(name)

    # returns True if the given abspath is a file that should be included in
    # the interesting file subset
    def contains(self, abspath):
        if not abspath.endswith(os.sep):
            abspath += os.sep
        for f in self.folders:
            if abspath.startswith(f):
                return True
        return False

class _Bzr:
    def getCommitTemplate(self, prefs, rev, names):
        # build command
        args = [ prefs.getString('bzr_bin'), 'log', '-v', '-r', rev ]
        # build list of interesting files
        pwd, isabs = os.path.abspath(os.curdir), False
        for name in names:
            isabs |= os.path.isabs(name)
            args.append(safeRelativePath(self.root, name, prefs, 'bzr_cygwin'))
        # run command
        ss = popenReadLines(self.root, args, prefs, 'bzr_bash')
        # parse response
        prev = 'before:' + rev
        fs = _VcsFolderSet(names)
        added, modified, removed, renamed = {}, {}, {}, {}
        i, n = 0, len(ss)
        while i < n:
            s = ss[i]
            i += 1
            if s.startswith('added:'):
                # added files
                while i < n and ss[i].startswith('  '):
                    k = prefs.convertToNativePath(ss[i][2:])
                    i += 1
                    if not k.endswith(os.sep):
                        k = os.path.join(self.root, k)
                        if fs.contains(k):
                            if not isabs:
                                k = relpath(pwd, k)
                            added[k] = [ (None, None), (k, rev) ]
            elif s.startswith('modified:'):
                # modified files
                while i < n and ss[i].startswith('  '):
                    k = prefs.convertToNativePath(ss[i][2:])
                    i += 1
                    if not k.endswith(os.sep):
                        k = os.path.join(self.root, k)
                        if fs.contains(k):
                            if not isabs:
                                k = relpath(pwd, k)
                            modified[k] = [ (k, prev), (k, rev) ]
            elif s.startswith('removed:'):
                # removed files
                while i < n and ss[i].startswith('  '):
                    k = prefs.convertToNativePath(ss[i][2:])
                    i += 1
                    if not k.endswith(os.sep):
                        k = os.path.join(self.root, k)
                        if fs.contains(k):
                            if not isabs:
                                k = relpath(pwd, k)
                            removed[k] = [ (k, prev), (None, None) ]
            elif s.startswith('renamed:'):
                # renamed files
                while i < n and ss[i].startswith('  '):
                    k = ss[i][2:].split(' => ')
                    i += 1
                    if len(k) == 2:
                        k0 = prefs.convertToNativePath(k[0])
                        k1 = prefs.convertToNativePath(k[1])
                        if not k0.endswith(os.sep) and not k1.endswith(os.sep):
                            k0 = os.path.join(self.root, k0)
                            k1 = os.path.join(self.root, k1)
                            if fs.contains(k0) or fs.contains(k1):
                                if not isabs:
                                    k0 = relpath(pwd, k0)
                                    k1 = relpath(pwd, k1)
                                renamed[k1] = [ (k0, prev), (k1, rev) ]
        # sort the results
        result, r = [], set()
        for m in removed, added, modified, renamed:
            r.update(m.keys())
        for k in sorted(r):
            for m in removed, added, modified, renamed:
                if k in m:
                    result.append(m[k])
        return result

    def getFolderTemplate(self, prefs, names):
        # build command
        args = [ prefs.getString('bzr_bin'), 'status', '-SV' ]
        # build list of interesting files
        pwd, isabs = os.path.abspath(os.curdir), False
        for name in names:
            isabs |= os.path.isabs(name)
            args.append(safeRelativePath(self.root, name, prefs, 'bzr_cygwin'))
        # run command
        prev = '-1'
        fs = _VcsFolderSet(names)
        added, modified, removed, renamed = {}, {}, {}, {}
        for s in popenReadLines(self.root, args, prefs, 'bzr_bash'):
            # parse response
            if len(s) < 5:
                continue
            y, k = s[1], s[4:]
            if y == 'D':
                # removed
                k = prefs.convertToNativePath(k)
                if not k.endswith(os.sep):
                    k = os.path.join(self.root, k)
                    if fs.contains(k):
                        if not isabs:
                            k = relpath(pwd, k)
                        removed[k] = [ (k, prev), (None, None) ]
            elif y == 'N':
                # added
                k = prefs.convertToNativePath(k)
                if not k.endswith(os.sep):
                    k = os.path.join(self.root, k)
                    if fs.contains(k):
                        if not isabs:
                            k = relpath(pwd, k)
                        added[k] = [ (None, None), (k, None) ]
            elif y == 'M':
                # modified or merge conflict
                k = prefs.convertToNativePath(k)
                if not k.endswith(os.sep):
                    k = os.path.join(self.root, k)
                    if fs.contains(k):
                        if not isabs:
                            k = relpath(pwd, k)
                        modified[k] = self.getFileTemplate(prefs, k)
            elif s[0] == 'R':
                # renamed
                k = k.split(' => ')
                if len(k) == 2:
                    k0 = prefs.convertToNativePath(k[0])
                    k1 = prefs.convertToNativePath(k[1])
                    if not k0.endswith(os.sep) and not k1.endswith(os.sep):
                        k0 = os.path.join(self.root, k0)
                        k1 = os.path.join(self.root, k1)
                        if fs.contains(k0) or fs.contains(k1):
                            if not isabs:
                                k0 = relpath(pwd, k0)
                                k1 = relpath(pwd, k1)
                            renamed[k1] = [ (k0, prev), (k1, None) ]
        # sort the results
        result, r = [], set()
        for m in removed, added, modified, renamed:
            r.update(m.keys())
        for k in sorted(r):
            for m in removed, added, modified, renamed:
                if k in m:
                    result.append(m[k])
        return result

    def getRevision(self, prefs, name, rev):
        return popenRead(self.root, [ prefs.getString('bzr_bin'), 'cat', '--name-from-revision', '-r', rev, safeRelativePath(self.root, name, prefs, 'bzr_cygwin') ], prefs, 'bzr_bash')

class _Cvs:
    def getCommitTemplate(self, prefs, rev, names):
        result = []
        try:
            r, prev = rev.split('.'), None
            if len(r) > 1:
                m = int(r.pop())
                if m > 1:
                    r.append(str(m - 1))
                else:
                    m = int(r.pop())
                if len(r):
                    prev = '.'.join(r)
            for k in sorted(names):
                 if prev is None:
                     k0 = None
                 else:
                     k0 = k
                 result.append([ (k0, prev), (k, rev) ])
        except ValueError:
            logError(_('Error parsing revision %s.') % (rev, ))
        return result

    def getFolderTemplate(self, prefs, names):
        # build command
        args = [ prefs.getString('cvs_bin'), '-nq', 'update', '-R' ]
        # build list of interesting files
        pwd, isabs = os.path.abspath(os.curdir), False
        for name in names:
            isabs |= os.path.isabs(name)
            args.append(safeRelativePath(self.root, name, prefs, 'cvs_cygwin'))
        # run command
        prev = 'BASE'
        fs = _VcsFolderSet(names)
        modified = {}
        for s in popenReadLines(self.root, args, prefs, 'cvs_bash'):
            # parse response
            if len(s) < 3 or s[0] not in 'ACMR':
                continue
            k = os.path.join(self.root, prefs.convertToNativePath(s[2:]))
            if fs.contains(k):
                if not isabs:
                    k = relpath(pwd, k)
                if s[0] == 'R':
                    # removed
                    modified[k] = [ (k, prev), (None, None) ]
                    pass
                elif s[0] == 'A':
                    # added
                    modified[k] = [ (None, None), (k, None) ]
                else:
                    # modified
                    modified[k] = [ (k, prev), (k, None) ]
        # sort the results
        return [ modified[k] for k in sorted(modified.keys()) ]

    def getRevision(self, prefs, name, rev):
        if rev == 'BASE' and not os.path.exists(name):
            # find revision for removed files
            for s in popenReadLines(self.root, [ prefs.getString('cvs_bin'), 'status', safeRelativePath(self.root, name, prefs, 'cvs_cygwin') ], prefs, 'cvs_bash'):
                if s.startswith('   Working revision:\t-'):
                    rev = s.split('\t')[1][1:]
        return popenRead(self.root, [ prefs.getString('cvs_bin'), '-Q', 'update', '-p', '-r', rev, safeRelativePath(self.root, name, prefs, 'cvs_cygwin') ], prefs, 'cvs_bash')

class _Darcs:
    def _getCommitTemplate(self, prefs, names, rev):
        mods = (rev is None)
        # build command
        args = [ prefs.getString('darcs_bin') ]
        if mods:
            args.extend(['whatsnew', '-s'])
        else:
            args.extend(['log', '--number', '-s'])
            try:
                args.extend(['-n', str(int(rev))])
            except ValueError:
                args.extend(['-h', rev])
        # build list of interesting files
        pwd, isabs = os.path.abspath(os.curdir), False
        for name in names:
            isabs |= os.path.isabs(name)
            if mods:
                args.append(safeRelativePath(self.root, name, prefs, 'darcs_cygwin'))
        # run command
        # 'darcs whatsnew' will return 1 if there are no changes
        ss = popenReadLines(self.root, args, prefs, 'darcs_bash', [0, 1])
        # parse response
        i, n = 0, len(ss)
        if mods:
            prev = ''
            rev = None
        else:
            try:
                rev = ss[0].split(':')[0]
                prev = str(int(rev) + 1)
                # skip to the beginning of the summary
                while i < n and len(ss[i]):
                    i += 1
            except (ValueError, IndexError):
                i = n
        fs = _VcsFolderSet(names)
        added, modified, removed, renamed = {}, {}, {}, {}
        while i < n:
            s = ss[i]
            i += 1
            if not mods:
                if s.startswith('    '):
                    s = s[4:]
                else:
                    continue
            if len(s) < 2:
                continue
            x = s[0]
            if x == 'R':
                # removed
                k = prefs.convertToNativePath(s[2:])
                if not k.endswith(os.sep):
                    k = os.path.join(self.root, k)
                    if fs.contains(k):
                        if not isabs:
                            k = relpath(pwd, k)
                        removed[k] = [ (k, prev), (None, None) ]
            elif x == 'A':
                # added
                k = prefs.convertToNativePath(s[2:])
                if not k.endswith(os.sep):
                    k = os.path.join(self.root, k)
                    if fs.contains(k):
                        if not isabs:
                            k = relpath(pwd, k)
                        added[k] = [ (None, None), (k, rev) ]
            elif x == 'M':
                # modified
                k = prefs.convertToNativePath(s[2:].split(' ')[0])
                if not k.endswith(os.sep):
                    k = os.path.join(self.root, k)
                    if fs.contains(k):
                        if not isabs:
                            k = relpath(pwd, k)
                        if k not in renamed:
                            modified[k] = [ (k, prev), (k, rev) ]
            elif x == ' ':
                # renamed
                k = s[1:].split(' -> ')
                if len(k) == 2:
                    k0 = prefs.convertToNativePath(k[0])
                    k1 = prefs.convertToNativePath(k[1])
                    if not k0.endswith(os.sep):
                        k0 = os.path.join(self.root, k0)
                        k1 = os.path.join(self.root, k1)
                        if fs.contains(k0) or fs.contains(k1):
                            if not isabs:
                                k0 = relpath(pwd, k0)
                                k1 = relpath(pwd, k1)
                            renamed[k1] = [ (k0, prev), (k1, rev) ]
        # sort the results
        result, r = [], set()
        for m in added, modified, removed, renamed:
            r.update(m.keys())
        for k in sorted(r):
            for m in removed, added, modified, renamed:
                if k in m:
                    result.append(m[k])
        return result

    def getCommitTemplate(self, prefs, rev, names):
        return self._getCommitTemplate(prefs, names, rev)

    def getFolderTemplate(self, prefs, names):
        return self._getCommitTemplate(prefs, names, None)

    def getRevision(self, prefs, name, rev):
        args = [ prefs.getString('darcs_bin'), 'show', 'contents' ]
        try:
            args.extend([ '-n', str(int(rev)) ])
        except ValueError:
            args.extend([ '-h', rev ])
        args.append(safeRelativePath(self.root, name, prefs, 'darcs_cygwin'))
        return popenRead(self.root, args, prefs, 'darcs_bash')

class _Git:
    def getCommitTemplate(self, prefs, rev, names):
        # build command
        args = [ prefs.getString('git_bin'), 'show', '--pretty=format:', '--name-status', rev ]
        # build list of interesting files
        pwd, isabs = os.path.abspath(os.curdir), False
        for name in names:
            isabs |= os.path.isabs(name)
        # run command
        prev = rev + '^'
        fs = _VcsFolderSet(names)
        modified = {}
        for s in popenReadLines(self.root, args, prefs, 'git_bash'):
            # parse response
            if len(s) < 2 or s[0] not in 'ADM':
                continue
            k = self._extractPath(s[2:], prefs)
            if fs.contains(k):
                if not isabs:
                    k = relpath(pwd, k)
                if s[0] == 'D':
                    # removed
                    modified[k] = [ (k, prev), (None, None) ]
                elif s[0] == 'A':
                    # added
                    modified[k] = [ (None, None), (k, rev) ]
                else:
                    # modified
                    modified[k] = [ (k, prev), (k, rev) ]
        # sort the results
        return [ modified[k] for k in sorted(modified.keys()) ]

    def _extractPath(self, s, prefs):
        return os.path.join(self.root, prefs.convertToNativePath(s.strip()))

    def getFolderTemplate(self, prefs, names):
        # build command
        args = [ prefs.getString('git_bin'), 'status', '--porcelain', '-s', '--untracked-files=no', '--ignore-submodules=all' ]
        # build list of interesting files
        pwd, isabs = os.path.abspath(os.curdir), False
        for name in names:
            isabs |= os.path.isabs(name)
        # run command
        prev = 'HEAD'
        fs = _VcsFolderSet(names)
        modified, renamed = {}, {}
        # 'git status' will return 1 when a commit would fail
        for s in popenReadLines(self.root, args, prefs, 'git_bash', [0, 1]):
            # parse response
            if len(s) < 3:
                continue
            x, y, k = s[0], s[1], s[2:]
            if x == 'R':
                # renamed
                k = k.split(' -> ')
                if len(k) == 2:
                    k0 = self._extractPath(k[0], prefs)
                    k1 = self._extractPath(k[1], prefs)
                    if fs.contains(k0) or fs.contains(k1):
                        if not isabs:
                            k0 = relpath(pwd, k0)
                            k1 = relpath(pwd, k1)
                        renamed[k1] = [ (k0, prev), (k1, None) ]
            elif x == 'U' or y == 'U' or (x == 'D' and y == 'D'):
                # merge conflict
                k = self._extractPath(k, prefs)
                if fs.contains(k):
                    if not isabs:
                        k = relpath(pwd, k)
                    if x == 'D':
                        panes = [ (None, None) ]
                    else:
                        panes = [ (k, ':2') ]
                    panes.append((k, None))
                    if y == 'D':
                        panes.append((None, None))
                    else:
                        panes.append((k, ':3'))
                    if x != 'A' and y != 'A':
                        panes.append((k, ':1'))
                    modified[k] = panes
            else:
                k = self._extractPath(k, prefs)
                if fs.contains(k):
                    if not isabs:
                        k = relpath(pwd, k)
                    if x == 'A':
                        # added
                        panes = [ (None, None) ]
                    else:
                        panes = [ (k, prev) ]
                    # staged changes
                    if x == 'D':
                        panes.append((None, None))
                    elif x != ' ':
                        panes.append((k, ':0'))
                    # working copy changes
                    if y == 'D':
                        panes.append((None, None))
                    elif y != ' ':
                        panes.append((k, None))
                    modified[k] = panes
        # sort the results
        result, r = [], set()
        for m in modified, renamed:
            r.update(m.keys())
        for k in sorted(r):
            for m in modified, renamed:
                if k in m:
                    result.append(m[k])
        return result

    def getRevision(self, prefs, name, rev):
        return popenRead(self.root, [ prefs.getString('git_bin'), 'show', '%s:%s' % (rev, relpath(self.root, os.path.abspath(name)).replace(os.sep, '/')) ], prefs, 'git_bash')

def _get_git_repo(path, prefs):
    if 'GIT_DIR' in os.environ:
        try:
            d = path
            ss = popenReadLines(d, [ prefs.getString('git_bin'), 'rev-parse', '--show-prefix' ], prefs, 'git_bash')
            if len(ss) > 0:
                # be careful to handle trailing slashes
                d = d.split(os.sep)
                if d[-1] != '':
                    d.append('')
                ss = strip_eol(ss[0]).split('/')
                if ss[-1] != '':
                    ss.append('')
                n = len(ss)
                if n <= len(d):
                    del d[-n:]
                if len(d) == 0:
                    d = os.curdir
                else:
                    d = os.sep.join(d)
            return _Git(d)
        except (IOError, OSError, WindowsError):
            # working tree not found
            pass
    # search for .git direcotry (project) or .git file (submodule)
    while True:
        name = os.path.join(path, '.git')
        if os.path.isdir(name) or os.path.isfile(name):
            return _Git(path)
        newpath = os.path.dirname(path)
        if newpath == path:
            break
        path = newpath

class _Hg:
    def _getPreviousRevision(self, prefs, rev):
        if rev is None:
            if self.working_rev is None:
                ss = popenReadLines(self.root, [ prefs.getString('hg_bin'), 'id', '-i', '-t' ], prefs, 'hg_bash')
                if len(ss) != 1:
                    raise IOError('Unknown working revision')
                ss = ss[0].split(' ')
                prev = ss[-1]
                if len(ss) == 1 and prev.endswith('+'):
                    # remove local modifications indicator
                    prev = prev[:-1]
                self.working_rev = prev
            return self.working_rev
        return 'p1(%s)' % (rev, )

    def getFileTemplate(self, prefs, name):
        return [ (name, self._getPreviousRevision(prefs, None)), (name, None) ]

    def _getCommitTemplate(self, prefs, names, cmd, rev):
        # build command
        args = [ prefs.getString('hg_bin') ]
        args.extend(cmd)
        # build list of interesting files
        pwd, isabs = os.path.abspath(os.curdir), False
        for name in names:
            isabs |= os.path.isabs(name)
            args.append(safeRelativePath(self.root, name, prefs, 'hg_cygwin'))
        # run command
        prev = self._getPreviousRevision(prefs, rev)
        fs = _VcsFolderSet(names)
        modified = {}
        for s in popenReadLines(self.root, args, prefs, 'hg_bash'):
            # parse response
            if len(s) < 3 or s[0] not in 'AMR':
                continue
            k = os.path.join(self.root, prefs.convertToNativePath(s[2:]))
            if fs.contains(k):
                if not isabs:
                    k = relpath(pwd, k)
                if s[0] == 'R':
                    # removed
                    modified[k] = [ (k, prev), (None, None) ]
                elif s[0] == 'A':
                    # added
                    modified[k] = [ (None, None), (k, rev) ]
                else:
                    # modified or merge conflict
                    modified[k] = [ (k, prev), (k, rev) ]
        # sort the results
        return [ modified[k] for k in sorted(modified.keys()) ]

    def getCommitTemplate(self, prefs, rev, names):
        return self._getCommitTemplate(prefs, names, [ 'log', '--template', 'A\t{file_adds}\nM\t{file_mods}\nR\t{file_dels}\n', '-r', rev ], rev)

    def getFolderTemplate(self, prefs, names):
        return self._getCommitTemplate(prefs, names, [ 'status', '-q' ], None)

    def getRevision(self, prefs, name, rev):
        return popenRead(self.root, [ prefs.getString('hg_bin'), 'cat', '-r', rev, safeRelativePath(self.root, name, prefs, 'hg_cygwin') ], prefs, 'hg_bash')

class _Mtn:
    def getFileTemplate(self, prefs, name):
        # FIXME: merge conflicts?
        return [ (name, 'h:'), (name, None) ]

    def getCommitTemplate(self, prefs, rev, names):
        # build command
        vcs_bin = prefs.getString('mtn_bin')
        ss = popenReadLines(self.root, [ vcs_bin, 'automate', 'select', '-q', rev ], prefs, 'mtn_bash')
        if len(ss) != 1:
            raise IOError('Ambiguous revision specifier')
        args = [ vcs_bin, 'automate', 'get_revision', ss[0] ]
        # build list of interesting files
        fs = _VcsFolderSet(names)
        pwd, isabs = os.path.abspath(os.curdir), False
        for name in names:
            isabs |= os.path.isabs(name)
        # run command
        prev = None
        removed, added, modified, renamed = {}, {}, {}, {}
        ss = popenReadLines(self.root, args, prefs, 'mtn_bash')
        i = 0
        while i < len(ss):
            # process results
            s = shlex_split(ss[i])
            i += 1
            if len(s) < 2:
                continue
            arg, arg1 = s[0], s[1]
            if arg == 'old_revision' and len(arg1) > 2:
                if prev is not None:
                    break
                prev = arg1[1:-1]
                continue
            elif prev is None:
                continue
            if arg == 'delete':
                # deleted file
                k = os.path.join(self.root, prefs.convertToNativePath(arg1))
                if fs.contains(k):
                    removed[arg1] = k
            elif arg == 'add_file':
                # new file
                k = os.path.join(self.root, prefs.convertToNativePath(arg1))
                if fs.contains(k):
                    added[arg1] = k
            elif arg == 'patch':
                # modified file
                k = os.path.join(self.root, prefs.convertToNativePath(arg1))
                if fs.contains(k):
                    modified[arg1] = k
            elif arg == 'rename':
                s = shlex_split(ss[i])
                i += 1
                if len(s) > 1 and s[0] == 'to':
                    # renamed file
                    k0 = os.path.join(self.root, prefs.convertToNativePath(arg1))
                    k1 = os.path.join(self.root, prefs.convertToNativePath(s[1]))
                    if fs.contains(k0) or fs.contains(k1):
                        renamed[s[1]] = (arg1, k0, k1)
        if removed or renamed:
            # remove directories
            removed_dirs = set()
            for s in popenReadLines(self.root, [ vcs_bin, 'automate', 'get_manifest_of', prev ], prefs, 'mtn_bash'):
                s = shlex_split(s)
                if len(s) > 1 and s[0] == 'dir':
                    removed_dirs.add(s[1])
            for k in removed_dirs:
                for m in removed, modified:
                    if k in m:
                        del m[k]
            for k, v in renamed.items():
                arg1, k0, k1 = v
                if arg1 in removed_dirs:
                    del renamed[k]
        # sort results
        result, r = [], set()
        for m in removed, added, modified, renamed:
            r.update(m)
        for k in sorted(r):
            if k in removed:
                k = removed[k]
                if not isabs:
                    k = relpath(pwd, k)
                result.append([ (k, prev), (None, None) ])
            elif k in added:
                k = added[k]
                if not isabs:
                    k = relpath(pwd, k)
                result.append([ (None, None), (k, rev) ])
            else:
                if k in renamed:
                    arg1, k0, k1 = renamed[k]
                else:
                    k0 = k1 = modified[k]
                if not isabs:
                    k0 = relpath(pwd, k0)
                    k1 = relpath(pwd, k1)
                result.append([ (k0, prev), (k1, rev) ])
        return result

    def getFolderTemplate(self, prefs, names):
        fs = _VcsFolderSet(names)
        result = []
        pwd, isabs = os.path.abspath(os.curdir), False
        args = [ prefs.getString('mtn_bin'), 'automate', 'inventory', '--no-ignored', '--no-unchanged', '--no-unknown' ]
        for name in names:
            isabs |= os.path.isabs(name)
        # build list of interesting files
        prev = 'h:'
        ss = popenReadLines(self.root, args, prefs, 'mtn_bash')
        removed, added, modified, renamed = {}, {}, {}, {}
        i = 0
        while i < len(ss):
            # parse properties
            m = {}
            while i < len(ss):
                s = ss[i]
                i += 1
                # properties are terminated by a blank line
                s = shlex_split(s)
                if len(s) == 0:
                    break
                m[s[0]] = s[1:]
            # scan the list of properties for files that interest us
            if len(m.get('path', [])) > 0:
                p, s, processed = m['path'][0], m.get('status', []), False
                if 'dropped' in s and 'file' in m.get('old_type', []):
                    # deleted file
                    k = os.path.join(self.root, prefs.convertToNativePath(p))
                    if fs.contains(k):
                        if not isabs:
                            k = relpath(pwd, k)
                        removed[k] = [ (k, prev), (None, None) ]
                    processed = True
                if 'added' in s and 'file' in m.get('new_type', []):
                    # new file
                    k = os.path.join(self.root, prefs.convertToNativePath(p))
                    if fs.contains(k):
                        if not isabs:
                            k = relpath(pwd, k)
                        added[k] = [ (None, None), (k, None) ]
                    processed = True
                if 'rename_target' in s and 'file' in m.get('new_type', []) and len(m.get('old_path', [])) > 0:
                    # renamed file
                    k0 = os.path.join(self.root, prefs.convertToNativePath(m['old_path'][0]))
                    k1 = os.path.join(self.root, prefs.convertToNativePath(p))
                    if fs.contains(k0) or fs.contains(k1):
                        if not isabs:
                            k0 = relpath(pwd, k0)
                            k1 = relpath(pwd, k1)
                        renamed[k1] = [ (k0, prev), (k1, None) ]
                    processed = True
                if not processed and 'file' in m.get('fs_type', []):
                    # modified file or merge conflict
                    k = os.path.join(self.root, prefs.convertToNativePath(p))
                    if fs.contains(k):
                        if not isabs:
                            k = relpath(pwd, k)
                        modified[k] = [ (k, prev), (k, None) ]
        # sort the results
        r = set()
        for m in removed, added, modified, renamed:
            r.update(m.keys())
        for k in sorted(r):
            for m in removed, added, modified, renamed:
                if k in m:
                    result.append(m[k])
        return result

    def getRevision(self, prefs, name, rev):
        return popenRead(self.root, [ prefs.getString('mtn_bin'), 'automate', 'get_file_of', '-q', '-r', rev, safeRelativePath(self.root, name, prefs, 'mtn_cygwin') ], prefs, 'mtn_bash')

class _Rcs:
    def getFileTemplate(self, prefs, name):
        args = [ prefs.getString('rcs_bin_rlog'), '-L', '-h', safeRelativePath(self.root, name, prefs, 'rcs_cygwin') ]
        rev = ''
        for line in popenReadLines(self.root, args, prefs, 'rcs_bash'):
            if line.startswith('head: '):
                rev = line[6:]
        return [ (name, rev), (name, None) ]

    def getCommitTemplate(self, prefs, rev, names):
        result = []
        try:
            r, prev = rev.split('.'), None
            if len(r) > 1:
                m = int(r.pop())
                if m > 1:
                    r.append(str(m - 1))
                else:
                    m = int(r.pop())
                if len(r):
                    prev = '.'.join(r)
            for k in sorted(names):
                 if prev is None:
                     k0 = None
                 else:
                     k0 = k
                 result.append([ (k0, prev), (k, rev) ])
        except ValueError:
            logError(_('Error parsing revision %s.') % (rev, ))
        return result

    def getFolderTemplate(self, prefs, names):
        # build command
        cmd = [ prefs.getString('rcs_bin_rlog'), '-L', '-h' ]
        # build list of interesting files
        pwd, isabs = os.path.abspath(os.curdir), False
        r = []
        for k in names:
            if os.path.isdir(k):
                # the user specified a folder
                n, ex = [ k ], True
                while len(n) > 0:
                    s = n.pop()
                    recurse = os.path.isdir(os.path.join(s, 'RCS'))
                    if ex or recurse:
                        ex = False
                        for v in os.listdir(s):
                            dn = os.path.join(s, v)
                            if v.endswith(',v') and os.path.isfile(dn):
                                # map to checkout name
                                r.append(dn[:-2])
                            elif v == 'RCS' and os.path.isdir(dn):
                                for v in os.listdir(dn):
                                    if os.path.isfile(os.path.join(dn, v)):
                                        if v.endswith(',v'):
                                            v = v[:-2]
                                        r.append(os.path.join(s, v))
                            elif recurse and os.path.isdir(dn) and not os.path.islink(dn):
                                n.append(dn)
            else:
                # the user specified a file
                s = k + ',v'
                if os.path.isfile(s):
                    r.append(k)
                    continue
                s = k.split(os.sep)
                s.insert(-1, 'RCS')
                # old-style RCS repository
                if os.path.isfile(os.sep.join(s)):
                    r.append(k)
                    continue
                # new-style RCS repository
                s[-1] += ',v'
                if os.path.isfile(os.sep.join(s)):
                    r.append(k)
        for k in r:
            isabs |= os.path.isabs(k)
        args = [ safeRelativePath(self.root, k, prefs, 'rcs_cygwin') for k in r ]
        # run command
        r, k = {}, ''
        for line in popenXArgsReadLines(self.root, cmd, args, prefs, 'rcs_bash'):
            # parse response
            if line.startswith('Working file: '):
                k = prefs.convertToNativePath(line[14:])
                k = os.path.join(self.root, os.path.normpath(k))
                if not isabs:
                    k = relpath(pwd, k)
            elif line.startswith('head: '):
                r[k] = line[6:]
        # sort the results
        return [ [ (k, r[k]), (k, None) ] for k in sorted(r.keys()) ]

    def getRevision(self, prefs, name, rev):
        return popenRead(self.root, [ prefs.getString('rcs_bin_co'), '-p', '-q', '-r' + rev, safeRelativePath(self.root, name, prefs, 'rcs_cygwin') ], prefs, 'rcs_bash')

def _get_rcs_repo(path, prefs):
    try:
        for s in os.listdir(path):
            if s.endswith(',v') and os.path.isfile(os.path.join(path, s)):
                return _Rcs(path)
    except OSError:
        # the user specified an invalid folder name
        pass

class _Svn:
    def _getVcs(self):
        return 'svn'

    def _getURLPrefix(self):
        return 'URL: '

    def _parseStatusLine(self, s):
        if len(s) < 8 or s[0] not in 'ACDMR':
            return
        # subversion 1.6 adds a new column
        k = 7
        if k < len(s) and s[k] == ' ':
            k += 1
        return s[0], s[k:]

    def _getPreviousRevision(self, rev):
        if rev is None:
            return 'BASE'
        m = int(rev)
        if m > 1:
            return str(m - 1)

    def _getURL(self, prefs):
        if self.url is None:
            vcs, prefix = self._getVcs(), self._getURLPrefix()
            n = len(prefix)
            args = [ prefs.getString(vcs + '_bin'), 'info' ]
            for s in popenReadLines(self.root, args, prefs, vcs + '_bash'):
                if s.startswith(prefix):
                    self.url = s[n:]
                    break
        return self.url

    def getFileTemplate(self, prefs, name):
        # FIXME: verify this
        # merge conflict
        escaped_name = globEscape(name)
        left = glob.glob(escaped_name + u'.merge-left.r*')
        right = glob.glob(escaped_name + u'.merge-right.r*')
        if len(left) > 0 and len(right) > 0:
            return [ (left[-1], None), (name, None), (right[-1], None) ]
        # update conflict
        left = sorted(glob.glob(escaped_name + u'.r*'))
        right = glob.glob(escaped_name + u'.mine')
        right.extend(glob.glob(escaped_name + u'.working'))
        if len(left) > 0 and len(right) > 0:
            return [ (left[-1], None), (name, None), (right[0], None) ]
        # default case
        return [ (name, self._getPreviousRevision(None)), (name, None) ]

    def _getCommitTemplate(self, prefs, rev, names):
        result = []
        try:
            prev = self._getPreviousRevision(rev)
        except ValueError, e:
            logError(_('Error parsing revision %s.') % (rev, ))
            return result

        # build command
        vcs = self._getVcs()
        vcs_bin, vcs_bash = prefs.getString(vcs + '_bin'), vcs + '_bash'
        if rev is None:
            args = [ vcs_bin, 'status', '-q' ]
        else:
            args = [ vcs_bin, 'diff', '--summarize', '-c', rev ]
        # build list of interesting files
        pwd, isabs = os.path.abspath(os.curdir), False
        for name in names:
            isabs |= os.path.isabs(name)
            if rev is None:
                args.append(safeRelativePath(self.root, name, prefs, vcs + '_cygwin'))
        # run command
        fs = _VcsFolderSet(names)
        modified, added, removed = {}, set(), set()
        for s in popenReadLines(self.root, args, prefs, vcs_bash):
            status = self._parseStatusLine(s)
            if status is None:
                continue
            v, k = status
            rel = prefs.convertToNativePath(k)
            k = os.path.join(self.root, rel)
            if fs.contains(k):
                if v == 'D':
                    # deleted file or directory
                    # the contents of deleted folders are not reported
                    # by "svn diff --summarize -c <rev>"
                    removed.add(rel)
                elif v == 'A':
                    # new file or directory
                    added.add(rel)
                elif v == 'M':
                    # modified file or merge conflict
                    k = os.path.join(self.root, k)
                    if not isabs:
                        k = relpath(pwd, k)
                    modified[k] = [ (k, prev), (k, rev) ]
                elif v == 'C':
                    # merge conflict
                    modified[k] = self.getFileTemplate(prefs, k)
                elif v == 'R':
                    # replaced file
                    removed.add(rel)
                    added.add(rel)
        # look for files in the added items
        if rev is None:
            m, added = added, {}
            for k in m:
                if not os.path.isdir(k):
                    # confirmed as added file
                    k = os.path.join(self.root, k)
                    if not isabs:
                        k = relpath(pwd, k)
                    added[k] = [ (None, None), (k, None) ]
        else:
            m = {}
            for k in added:
                d, b = os.path.dirname(k), os.path.basename(k)
                if d not in m:
                    m[d] = set()
                m[d].add(b)
            # remove items we can easily determine to be directories
            for k in m.keys():
                d = os.path.dirname(k)
                if d in m:
                    m[d].discard(os.path.basename(k))
                    if not m[d]:
                        del m[d]
            # determine which are directories
            added = {}
            for p, v in m.items():
                for s in popenReadLines(self.root, [ vcs_bin, 'list', '-r', rev, '%s/%s' % (self._getURL(prefs), p.replace(os.sep, '/')) ], prefs, vcs_bash):
                    if s in v:
                        # confirmed as added file
                        k = os.path.join(self.root, os.path.join(p, s))
                        if not isabs:
                            k = relpath(pwd, k)
                        added[k] = [ (None, None), (k, rev) ]
        # determine if removed items are files or directories
        if prev == 'BASE':
            m, removed = removed, {}
            for k in m:
                if not os.path.isdir(k):
                    # confirmed item as file
                    k = os.path.join(self.root, k)
                    if not isabs:
                        k = relpath(pwd, k)
                    removed[k] = [ (k, prev), (None, None) ]
        else:
            m = {}
            for k in removed:
                d, b = os.path.dirname(k), os.path.basename(k)
                if d not in m:
                    m[d] = set()
                m[d].add(b)
            removed_dir, removed = set(), {}
            for p, v in m.items():
                for s in popenReadLines(self.root, [ vcs_bin, 'list', '-r', prev, '%s/%s' % (self._getURL(prefs), p.replace(os.sep, '/')) ], prefs, vcs_bash):
                    if s.endswith('/'):
                        s = s[:-1]
                        if s in v:
                            # confirmed item as directory
                            removed_dir.add(os.path.join(p, s))
                    else:
                        if s in v:
                            # confirmed item as file
                            k = os.path.join(self.root, os.path.join(p, s))
                            if not isabs:
                                k = relpath(pwd, k)
                            removed[k] = [ (k, prev), (None, None) ]
            # recursively find all unreported removed files
            while removed_dir:
                tmp = removed_dir
                removed_dir = set()
                for p in tmp:
                    for s in popenReadLines(self.root, [ vcs_bin, 'list', '-r', prev, '%s/%s' % (self._getURL(prefs), p.replace(os.sep, '/')) ], prefs, vcs_bash):
                        if s.endswith('/'):
                            # confirmed item as directory
                            removed_dir.add(os.path.join(p, s[:-1]))
                        else:
                            # confirmed item as file
                            k = os.path.join(self.root, os.path.join(p, s))
                            if not isabs:
                                k = relpath(pwd, k)
                            removed[k] = [ (k, prev), (None, None) ]
        # sort the results
        r = set()
        for m in removed, added, modified:
            r.update(m.keys())
        for k in sorted(r):
            for m in removed, added, modified:
                if k in m:
                    result.append(m[k])
        return result

    def getCommitTemplate(self, prefs, rev, names):
        return self._getCommitTemplate(prefs, rev, names)

    def getFolderTemplate(self, prefs, names):
        return self._getCommitTemplate(prefs, None, names)

    def getRevision(self, prefs, name, rev):
        vcs_bin = prefs.getString('svn_bin')
        if rev in [ 'BASE', 'COMMITTED', 'PREV' ]:
            return popenRead(self.root, [ vcs_bin, 'cat', '%s@%s' % (safeRelativePath(self.root, name, prefs, 'svn_cygwin'), rev) ], prefs, 'svn_bash')
        return popenRead(self.root, [ vcs_bin, 'cat', '%s/%s@%s' % (self._getURL(prefs), relpath(self.root, os.path.abspath(name)).replace(os.sep, '/'), rev) ], prefs, 'svn_bash')

class _Svk(_Svn):
    def _getVcs(self):
        return 'svk'

    def _getURLPrefix(self):
        return 'Depot Path: '

    def _parseStatusLine(self, s):
        if len(s) < 4 or s[0] not in 'ACDMR':
            return
        return s[0], s[4:]

    def _getPreviousRevision(self, rev):
        if rev is None:
            return 'HEAD'
        if rev.endswith('@'):
            return str(int(rev[:-1]) - 1) + '@'
        return str(int(rev) - 1)

    def getRevision(self, prefs, name, rev):
        return popenRead(self.root, [ prefs.getString('svk_bin'), 'cat', '-r', rev, '%s/%s' % (self._getURL(prefs), relpath(self.root, os.path.abspath(name)).replace(os.sep, '/')) ], prefs, 'svk_bash')

def _get_svk_repo(path, prefs):
    name = path
    # parse the ~/.svk/config file to discover which directories are part of
    # SVK repositories
    if isWindows():
        name = name.upper()
    svkroot = os.environ.get('SVKROOT', None)
    if svkroot is None:
        svkroot = os.path.expanduser('~/.svk')
    svkconfig = os.path.join(svkroot, 'config')
    if os.path.isfile(svkconfig):
        try:
            # find working copies by parsing the config file
            f = open(svkconfig, 'r')
            ss = readlines(f)
            f.close()
            projs, sep = [], os.sep
            # find the separator character
            for s in ss:
                if s.startswith('  sep: ') and len(s) > 7:
                    sep = s[7]
            # find the project directories
            i = 0
            while i < len(ss):
                s = ss[i]
                i += 1
                if s.startswith('  hash: '):
                    while i < len(ss) and ss[i].startswith('    '):
                        s = ss[i]
                        i += 1
                        if s.endswith(': ') and i < len(ss) and ss[i].startswith('      depotpath: '):
                            key = s[4:-2].replace(sep, os.sep)
                            # parse directory path
                            j, n, tt = 0, len(key), []
                            while j < n:
                                if key[j] == '"':
                                    # quoted string
                                    j += 1
                                    while j < n:
                                        if key[j] == '"':
                                            j += 1
                                            break
                                        elif key[j] == '\\':
                                            # escaped character
                                            j += 1
                                        if j < n:
                                            tt.append(key[j])
                                            j += 1
                                else:
                                    tt.append(key[j])
                                    j += 1
                            key = ''.join(tt).replace(sep, os.sep)
                            if isWindows():
                                key = key.upper()
                            projs.append(key)
                    break
            # check if the file belongs to one of the project directories
            if _VcsFolderSet(projs).contains(name):
                return _Svk(path)
        except IOError:
            logError(_('Error parsing %s.') % (svkconfig, ))

class VCSs:
    def __init__(self):
        # initialise the VCS objects
        self._get_repo = { 'bzr': _get_bzr_repo, 'cvs': _get_cvs_repo, 'darcs': _get_darcs_repo, 'git': _get_git_repo, 'hg': _get_hg_repo, 'mtn': _get_mtn_repo, 'rcs': _get_rcs_repo, 'svk': _get_svk_repo, 'svn': _get_svn_repo }

    def setSearchOrder(self, ordering):
        self._search_order = ordering

    def findByFolder(self, path, prefs):
        path = os.path.abspath(path)
        for vcs in prefs.getString('vcs_search_order').split():
            if vcs in self._get_repo:
                repo = self._get_repo[vcs](path, prefs)
                if repo:
                    return repo

# Enforcing manual alignment is accomplished by dividing the lines of text into
# sections that are matched independently.  'blocks' is an array of integers
# describing how many lines (including null lines for spacing) that are in each
# section starting from the begining of the files.  When no manual alignment
# has been specified, all lines are in the same section so 'blocks' should
# contain a single entry equal to the number of lines.  Zeros are not allowed
# in this array so 'blocks' will be an empty array when there are no lines.  A
# 'cut' at location 'i' means a line 'i-1' and line 'i' belong to different
# sections

def createBlock(n):
    if n > 0:
        return [ n ]
    return []

# returns the two sets of blocks after cutting at 'i'
def cutBlocks(i, blocks):
    pre, post, nlines = [], [], 0
    for b in blocks:
        if nlines >= i:
            post.append(b)
        elif nlines + b <= i:
            pre.append(b)
        else:
            n = i - nlines
            pre.append(n)
            post.append(b - n)
        nlines += b
    return pre, post

# returns a set of blocks containing all of the cuts in the inputs
def mergeBlocks(leftblocks, rightblocks):
    leftblocks, rightblocks, b = leftblocks[:], rightblocks[:], []
    while len(leftblocks) > 0:
        nleft, nright = leftblocks[0], rightblocks[0]
        n = min(nleft, nright)
        if n < nleft:
            leftblocks[0] -= n
        else:
            del leftblocks[0]
        if n < nright:
            rightblocks[0] -= n
        else:
            del rightblocks[0]
        b.append(n)
    return b

# utility method to simplify working with structures used to describe character
# differences of a line
#
# ranges of character differences are indicated by (start, end, flags) tuples
# where 'flags' is a mask used to indicate if the characters are different from
# the line to the left, right, or both
#
# this method will return the union of two sorted lists of ranges
def mergeRanges(r1, r2):
    r1, r2, result, start = r1[:], r2[:], [], 0
    rs = [ r1, r2 ]
    while len(r1) > 0 and len(r2) > 0:
        flags, start = 0, min(r1[0][0], r2[0][0])
        if start == r1[0][0]:
            r1end = r1[0][1]
            flags |= r1[0][2]
        else:
            r1end = r1[0][0]
        if start == r2[0][0]:
            r2end = r2[0][1]
            flags |= r2[0][2]
        else:
            r2end = r2[0][0]
        end = min(r1end, r2end)
        result.append((start, end, flags))
        for r in rs:
            if start == r[0][0]:
                if end == r[0][1]:
                    del r[0]
                else:
                    r[0] = (end, r[0][1], r[0][2])
    result.extend(r1)
    result.extend(r2)
    return result

# eliminates lines that are spacing lines in all panes
def removeNullLines(blocks, lines_set):
    bi, bn, i = 0, 0, 0
    while bi < len(blocks):
        while i < bn + blocks[bi]:
            for lines in lines_set:
                if lines[i] is not None:
                    i += 1
                    break
            else:
                for lines in lines_set:
                    del lines[i]
                blocks[bi] -= 1
        if blocks[bi] == 0:
            del blocks[bi]
        else:
            bn += blocks[bi]
            bi += 1

def nullToEmpty(s):
    if s is None:
        s = ''
    return s

# returns true if the string only contains whitespace characters
def isBlank(s):
    for c in whitespace:
        s = s.replace(c, '')
    return len(s) == 0

# use pango.SCALE instead of pango.PIXELS to avoid overflow exception
def pixels(size):
    return int(size / pango.SCALE + 0.5)

# constructs a full URL for the named file
def path2url(path, proto='file'):
    r = [ proto, ':///' ]
    s = os.path.abspath(path)
    i = 0
    while i < len(s) and s[i] == os.sep:
        i += 1
    for c in s[i:]:
        if c == os.sep:
            c = '/'
        elif c == ':' and isWindows():
            c = '|'
        else:
            v = ord(c)
            if v <= 0x20 or v >= 0x7b or c in '$&+,/:;=?@"<>#%\\^[]`':
                c = '%%%02X' % (v, )
        r.append(c)
    return ''.join(r)

# returns a path to a file named by the file URL 's'
def url2path(s):
    n = len(s)
    if n > 7 and s.startswith('file://'):
        ss, i = [], 7
        if isWindows():
            if s[i] == '/':
                i += 1
            else:
                # share
                ss.append('//')
        while i < n:
            c = s[i]
            i += 1
            if c == '%':
                v = 0
                for j in 0, 1:
                    if i < n:
                        p = ord(s[i])
                        if p >= 48 and p <= 57:
                            p -= 48
                        elif p >= 65 and p <= 70:
                            p -= 55
                        elif p >= 97 and p <= 102:
                            p -= 87
                        else:
                            p = None
                        if p is not None:
                            v = 16 * v + p
                            i += 1
                c = chr(v)
            elif c == '|' and isWindows():
                c = ':'
            ss.append(c)
        s = ''.join(ss)
        return s.replace('/', os.sep)

# mapping to column width of a character (tab will never be in this map)
char_width_cache = {}

ALPHANUMERIC_CLASS = 0
WHITESPACE_CLASS = 1
OTHER_CLASS = 2

# maps similar types of characters to a group
def getCharacterClass(c):
    if c.isalnum() or c == u'_':
        return ALPHANUMERIC_CLASS
    elif c.isspace():
        return WHITESPACE_CLASS
    return OTHER_CLASS

# longest common subsequence of unique elements common to 'a' and 'b'
def __patience_subsequence(a, b):
    # value unique lines by their order in each list
    value_a, value_b = {}, {}
    # find unique values in 'a'
    for i, s in enumerate(a):
        if s in value_a:
            value_a[s] = -1
        else:
            value_a[s] = i
    # find unique values in 'b'
    for i, s in enumerate(b):
        if s in value_b:
            value_b[s] = -1
        else:
            value_b[s] = i
    # lay down items in 'b' as if playing patience if the item is unique in
    # 'a' and 'b'
    pile, pointers, atob = [], {}, {}
    get, append = value_a.get, pile.append
    for s in b:
        v = get(s, -1)
        if v != -1:
            vb = value_b[s]
            if vb != -1:
                atob[v] = vb
                # find appropriate pile for v
                start, end = 0, len(pile)
                # optimisation as values usually increase
                if end and v > pile[-1]:
                    start = end
                else:
                    while start < end:
                        mid = (start + end) // 2
                        if v < pile[mid]:
                            end = mid
                        else:
                            start = mid + 1
                if start < len(pile):
                    pile[start] = v
                else:
                    append(v)
                if start:
                    pointers[v] = pile[start-1]
    # examine our piles to determine the longest common subsequence
    result = []
    if pile:
        v, append = pile[-1], result.append
        append((v, atob[v]))
        while v in pointers:
            v = pointers[v]
            append((v, atob[v]))
        result.reverse()
    return result

# difflib-style approximation of the longest common subsequence
def __lcs_approx(a, b):
    count1, lookup = {}, {}
    # count occurances of each element in 'a'
    for s in a:
        count1[s] = count1.get(s, 0) + 1
    # construct a mapping from a element to where it can be found in 'b'
    for i, s in enumerate(b):
        if s in lookup:
            lookup[s].append(i)
        else:
            lookup[s] = [ i ]
    if set(lookup).intersection(count1):
        # we have some common elements
        # identify popular entries
        popular = {}
        n = len(a)
        if n > 200:
            for k, v in count1.items():
                if 100 * v > n:
                    popular[k] = 1
        n = len(b)
        if n > 200:
            for k, v in lookup.items():
                if 100 * len(v) > n:
                    popular[k] = 1
        # while walk through entries in 'a', incrementally update the list of
        # matching subsequences in 'b' and keep track of the longest match
        # found
        prev_matches, matches, max_length, max_indices = {}, {}, 0, []
        for ai, s in enumerate(a):
            if s in lookup:
                if s in popular:
                    # we only extend existing previously found matches to avoid
                    # performance issues
                    for bi in prev_matches:
                        if bi + 1 < n and b[bi + 1] == s:
                            matches[bi] = v = prev_matches[bi] + 1
                            # check if this is now the longest match
                            if v >= max_length:
                                if v == max_length:
                                    max_indices.append((ai, bi))
                                else:
                                    max_length = v
                                    max_indices = [ (ai, bi) ]
                else:
                    prev_get = prev_matches.get
                    for bi in lookup[s]:
                        matches[bi] = v = prev_get(bi - 1, 0) + 1
                        # check if this is now the longest match
                        if v >= max_length:
                            if v == max_length:
                                max_indices.append((ai, bi))
                            else:
                                max_length = v
                                max_indices = [ (ai, bi) ]
            prev_matches, matches = matches, {}
        if max_indices:
            # include any popular entries at the beginning
            aidx, bidx, nidx = 0, 0, 0
            for ai, bi in max_indices:
                n = max_length
                ai += 1 - n
                bi += 1 - n
                while ai and bi and a[ai - 1] == b[bi - 1]:
                    ai -= 1
                    bi -= 1
                    n += 1
                if n > nidx:
                    aidx, bidx, nidx = ai, bi, n
            return aidx, bidx, nidx

# patinence diff with difflib-style fallback
def patience_diff(a, b):
    matches, len_a, len_b = [], len(a), len(b)
    if len_a and len_b:
        blocks = [ (0, len_a, 0, len_b, 0) ]
        while blocks:
            start_a, end_a, start_b, end_b, match_idx = blocks.pop()
            aa, bb = a[start_a:end_a], b[start_b:end_b]
            # try patience
            pivots = __patience_subsequence(aa, bb)
            if pivots:
                offset_a, offset_b = start_a, start_b
                for pivot_a, pivot_b in pivots:
                    pivot_a += offset_a
                    pivot_b += offset_b
                    if start_a <= pivot_a:
                        # extend before
                        idx_a, idx_b = pivot_a, pivot_b
                        while start_a < idx_a and start_b < idx_b and a[idx_a - 1] == b[idx_b - 1]:
                            idx_a -= 1
                            idx_b -= 1
                        # if anything is before recurse on the section
                        if start_a < idx_a and start_b < idx_b:
                            blocks.append((start_a, idx_a, start_b, idx_b, match_idx))
                        # extend after
                        start_a, start_b = pivot_a + 1, pivot_b + 1
                        while start_a < end_a and start_b < end_b and a[start_a] == b[start_b]:
                            start_a += 1
                            start_b += 1
                        # record match
                        matches.insert(match_idx, (idx_a, idx_b, start_a - idx_a))
                        match_idx += 1
                # if anything is after recurse on the section
                if start_a < end_a and start_b < end_b:
                    blocks.append((start_a, end_a, start_b, end_b, match_idx))
            else:
                # fallback if patience fails
                pivots = __lcs_approx(aa, bb)
                if pivots:
                    idx_a, idx_b, n = pivots
                    idx_a += start_a
                    idx_b += start_b
                    # if anything is before recurse on the section
                    if start_a < idx_a and start_b < idx_b:
                        blocks.append((start_a, idx_a, start_b, idx_b, match_idx))
                    # record match
                    matches.insert(match_idx, (idx_a, idx_b, n))
                    match_idx += 1
                    idx_a += n
                    idx_b += n
                    # if anything is after recurse on the section
                    if idx_a < end_a and idx_b < end_b:
                        blocks.append((idx_a, end_a, idx_b, end_b, match_idx))
    # try matching from begining to first match block
    if matches:
        end_a, end_b = matches[0][:2]
    else:
        end_a, end_b = len_a, len_b
    i = 0
    while i < end_a and i < end_b and a[i] == b[i]:
        i += 1
    if i:
        matches.insert(0, (0, 0, i))
    # try matching from last match block to end
    if matches:
        start_a, start_b, n = matches[-1]
        start_a += n
        start_b += n
    else:
        start_a, start_b = 0, 0
    end_a, end_b = len_a, len_b
    while start_a < end_a and start_b < end_b and a[end_a - 1] == b[end_b - 1]:
        end_a -= 1
        end_b -= 1
    if end_a < len_a:
        matches.append((end_a, end_b, len_a - end_a))
    # add a zero length block to the end
    matches.append((len_a, len_b, 0))
    return matches

class FileDiffViewer(gtk.Table):
    def __init__(self, n, prefs):
        # keybindings
        self._line_mode_actions = {
            'down': self._line_mode_down,
            'extend_down': self._line_mode_extend_down,
            'left': self._line_mode_left,
            'extend_left': self._line_mode_extend_left,
            'right': self._line_mode_right,
            'extend_right': self._line_mode_extend_right,
            'page_up': self._line_mode_page_up,
            'extend_page_up': self._line_mode_extend_page_up,
            'page_down': self._line_mode_page_down,
            'extend_page_down': self._line_mode_extend_page_down,
            'delete_text': self._delete_text,
            'clear_edits': self.clear_edits,
            'isolate': self.isolate,
            'first_difference': self.first_difference,
            'previous_difference': self.previous_difference,
            'next_difference': self.next_difference,
            'last_difference': self.last_difference,
            'copy_selection_right': self.copy_selection_right,
            'copy_selection_left': self.copy_selection_left,
            'copy_left_into_selection': self.copy_left_into_selection,
            'copy_right_into_selection': self.copy_right_into_selection,
            'merge_from_left_then_right': self.merge_from_left_then_right,
            'merge_from_right_then_left': self.merge_from_right_then_left }
        self._button_actions = {
            'undo': self.undo,
            'redo': self.redo,
            'cut': self.cut,
            'copy': self.copy,
            'paste': self.paste,
            'select_all': self.select_all,
            'clear_edits': self.clear_edits,
            'dismiss_all_edits': self.dismiss_all_edits,
            'realign_all': self.realign_all,
            'isolate': self.isolate,
            'first_difference': self.first_difference,
            'previous_difference': self.previous_difference,
            'next_difference': self.next_difference,
            'last_difference': self.last_difference,
            'shift_pane_right': self.shift_pane_right,
            'shift_pane_left': self.shift_pane_left,
            'convert_to_upper_case': self.convert_to_upper_case,
            'convert_to_lower_case': self.convert_to_lower_case,
            'sort_lines_in_ascending_order': self.sort_lines_in_ascending_order,
            'sort_lines_in_descending_order': self.sort_lines_in_descending_order,
            'remove_trailing_white_space': self.remove_trailing_white_space,
            'convert_tabs_to_spaces': self.convert_tabs_to_spaces,
            'convert_leading_spaces_to_tabs': self.convert_leading_spaces_to_tabs,
            'increase_indenting': self.increase_indenting,
            'decrease_indenting': self.decrease_indenting,
            'convert_to_dos': self.convert_to_dos,
            'convert_to_mac': self.convert_to_mac,
            'convert_to_unix': self.convert_to_unix,
            'copy_selection_right': self.copy_selection_right,
            'copy_selection_left': self.copy_selection_left,
            'copy_left_into_selection': self.copy_left_into_selection,
            'copy_right_into_selection': self.copy_right_into_selection,
            'merge_from_left_then_right': self.merge_from_left_then_right,
            'merge_from_right_then_left': self.merge_from_right_then_left }

        # create panes
        self.dareas = []
        self.panes = []
        self.hadj = gtk.Adjustment()
        self.vadj = gtk.Adjustment()
        for i in range(n):
            pane = FileDiffViewer.Pane()
            self.panes.append(pane)

            # pane contents
            sw = ScrolledWindow(self.hadj, self.vadj)
            darea = sw.darea
            darea.add_events(gtk.gdk.BUTTON_PRESS_MASK | gtk.gdk.BUTTON1_MOTION_MASK)
            darea.connect('button_press_event', self.darea_button_press_cb, i)
            darea.connect('motion_notify_event', self.darea_motion_notify_cb, i)
            darea.connect('expose_event', self.darea_expose_cb, i)
            self.dareas.append(darea)
            self.attach(sw, i, i + 1, 1, 2)
            sw.show()

        self.hadj.connect('value_changed', self.hadj_changed_cb)
        self.vadj.connect('value_changed', self.vadj_changed_cb)

        # add diff map
        self.map = map = gtk.DrawingArea()
        map.add_events(gtk.gdk.BUTTON_PRESS_MASK | gtk.gdk.BUTTON1_MOTION_MASK)
        map.connect('button_press_event', self.map_button_press_cb)
        map.connect('motion_notify_event', self.map_button_press_cb)
        map.connect('scroll_event', self.map_scroll_cb)
        map.connect('expose_event', self.map_expose_cb)
        self.attach(map, n, n + 1, 1, 2, gtk.FILL, gtk.FILL)
        map.show()
        map.set_size_request(16 * n, 0)
        self.add_events(gtk.gdk.KEY_PRESS_MASK | gtk.gdk.FOCUS_CHANGE)
        self.connect('focus_in_event', self.focus_in_cb)
        self.connect('focus_out_event', self.focus_out_cb)
        self.connect('key_press_event', self.key_press_cb)

        # input method
        self.im_context = im = gtk.IMMulticontext()
        im.connect('commit', self.im_commit_cb)
        im.connect('preedit_changed', self.im_preedit_changed_cb)
        im.connect('retrieve_surrounding', self.im_retrieve_surrounding_cb)
        self.im_preedit = None
        # Cache our keyboard focus state so we know when to change the
        # input method's focus.  We ensure the input method is given focus
        # if and only if (self.has_focus and self.mode == CHAR_MODE).
        self.has_focus = False

        # font
        self.setFont(pango.FontDescription(prefs.getString('display_font')))
        self.cursor_pos = (0, 0)

        # scroll to first difference when realised
        darea.connect_after('realize', self._realise_cb)

    # callback used when the viewer is first displayed
    # this must be connected with 'connect_after()' so the final widget sizes
    # are known and the scroll bar can be moved to the first difference
    def _realise_cb(self, widget):
        self.im_context.set_client_window(self.window)
        try:
            self.go_to_line(self.options['line'])
        except KeyError:
            self.first_difference()

    # callback for most menu items and buttons
    def button_cb(self, widget, data):
        self.openUndoBlock()
        self._button_actions[data]()
        self.closeUndoBlock()

    # updates the display font and resizes viewports as necessary
    def setFont(self, font):
        self.font = font
        metrics = self.get_pango_context().get_metrics(self.font)
        self.font_height = max(pixels(metrics.get_ascent() + metrics.get_descent()), 1)
        self.digit_width = metrics.get_approximate_digit_width()
        self.updateSize(True)
        self.map.queue_draw()

    # returns the 'column width' for a string -- used to help position
    # characters when tabs and other special characters are present
    # This is an inline loop over self.characterWidth() for performance reasons.
    def stringWidth(self, s):
        if not self.prefs.getBool('display_show_whitespace'):
            s = strip_eol(s)
        col = 0
        for c in s:
            try:
                w = char_width_cache[c]
            except KeyError:
                v = ord(c)
                if v < 32:
                    if c == '\t':
                        tab_width = self.prefs.getInt('display_tab_width')
                        w = tab_width - col % tab_width
                    elif c == '\n':
                        w = 1
                        char_width_cache[c] = w
                    else:
                        w = 2
                        char_width_cache[c] = w
                else:
                    if unicodedata.east_asian_width(c) in 'WF':
                        w = 2
                    else:
                        w = 1
                    char_width_cache[c] = w
            col += w
        return col

    # returns the 'column width' for a single character created at column 'i'
    def characterWidth(self, i, c):
        try:
            return char_width_cache[c]
        except KeyError:
            v = ord(c)
            if v < 32:
                if c == '\t':
                    tab_width = self.prefs.getInt('display_tab_width')
                    return tab_width - i % tab_width
                elif c == '\n':
                    w = 1
                else:
                    w = 2
            elif unicodedata.east_asian_width(c) in 'WF':
                w = 2
            else:
                w = 1
            char_width_cache[c] = w
            return w

    # translates a string into an array of the printable representation for
    # each character
    def expand(self, s):
        visible = self.prefs.getBool('display_show_whitespace')
        if not visible:
            s = strip_eol(s)
        tab_width = self.prefs.getInt('display_tab_width')
        col = 0
        result = []
        for c in s:
            v = ord(c)
            if v <= 32:
                if c == ' ':
                    if visible:
                        # show spaces using a centre-dot
                        result.append(u'\u00b7');
                    else:
                        result.append(c);
                elif c == '\t':
                    width = tab_width - col % tab_width
                    if visible:
                        # show tabs using a right double angle quote
                        result.append(u'\u00bb' + (width - 1) * u' ')
                    else:
                        result.append(width * u' ')
                elif c == '\n' and visible:
                    # show newlines using a pilcrow
                    result.append(u'\u00b6')
                else:
                    # prefix with a ^
                    result.append(u'^' + chr(v + 64))
            else:
                result.append(c)
            col += self.characterWidth(col, c)
        return result

    def setLineMode(self):
        if self.mode != LINE_MODE:
            if self.mode == CHAR_MODE:
                self._im_focus_out()
                self.im_context.reset()
                self._im_set_preedit(None)
                self.current_char = 0
                self.selection_char = 0
                self.dareas[self.current_pane].queue_draw()
            elif self.mode == ALIGN_MODE:
                self.dareas[self.align_pane].queue_draw()
                self.dareas[self.current_pane].queue_draw()
                self.align_pane = 0
                self.align_line = 0
            self.mode = LINE_MODE
            self.emit('cursor_changed')
            self.emit('mode_changed')

    def setCharMode(self):
        if self.mode != CHAR_MODE:
            if self.mode == LINE_MODE:
                self.cursor_column = -1
                self.setCurrentChar(self.current_line, 0)
            elif self.mode == ALIGN_MODE:
                self.dareas[self.align_pane].queue_draw()
                self.cursor_column = -1
                self.align_pane = 0
                self.align_line = 0
                self.setCurrentChar(self.current_line, 0)
            self._im_focus_in()
            self.im_context.reset()
            self.mode = CHAR_MODE
            self.emit('cursor_changed')
            self.emit('mode_changed')

    # sets the syntax hightlighting rules
    def setSyntax(self, s):
        if self.syntax is not s:
            self.syntax = s
            # invalidate the syntax caches
            for pane in self.panes:
                pane.syntax_cache = []
            self.emit('syntax_changed', s)
            # force all panes to redraw
            for darea in self.dareas:
                darea.queue_draw()

    # gets the syntax
    def getSyntax(self):
        return self.syntax

    # returns True if any pane contains edits
    def hasEdits(self):
        for pane in self.panes:
            if pane.num_edits > 0:
                return True
        return False

    # Changes to the diff viewer's state is recorded so they can be later
    # undone.  The recorded changes are organised into blocks that correspond
    # to high level action initiated by the user.  For example, pasting some
    # text may modify some lines and cause insertion of spacing lines to keep
    # proper alignment with the rest of the panes.  An undo operation initiated
    # by the user should undo all of these changes in a single step.
    # openUndoBlock() should be called when the action from a user, like a
    # mouse button press, menu item, etc. may cause change to the diff viewer's
    # state
    def openUndoBlock(self):
        self.undoblock = []

    # all changes to the diff viewer's state should create an Undo object and
    # add it to the current undo block using this method
    # this method does not need to be called when the state change is a result
    # of an undo/redo operation (self.undoblock is None in these cases)
    def addUndo(self, u):
        self.undoblock.append(u)

    # all openUndoBlock() calls should also have a matching closeUndoBlock()
    # this method collects all Undos created since the openUndoBlock() call
    # and pushes them onto the undo stack as a single unit
    def closeUndoBlock(self):
        if len(self.undoblock) > 0:
            self.redos = []
            self.undos.append(self.undoblock)
        self.undoblock = None

    # 'undo' action
    def undo(self):
        self.undoblock, old_block = None, self.undoblock
        if self.mode == CHAR_MODE:
            # avoid implicit preedit commit when an undo changes the mode
            self.im_context.reset()
        if self.mode == LINE_MODE or self.mode == CHAR_MODE:
            if len(self.undos) > 0:
                # move the block to the redo stack
                block = self.undos.pop()
                self.redos.append(block)
                # undo all changes in the block in reverse order
                for u in block[::-1]:
                    u.undo(self)
        self.undoblock = old_block

    # 'redo' action
    def redo(self):
        self.undoblock, old_block = None, self.undoblock
        if self.mode == LINE_MODE or self.mode == CHAR_MODE:
            if self.mode == CHAR_MODE:
                # avoid implicit preedit commit when an redo changes the mode
                self.im_context.reset()
            if len(self.redos) > 0:
                # move the block to the undo stack
                block = self.redos.pop()
                self.undos.append(block)
                # re-apply all changes in the block
                for u in block:
                    u.redo(self)
        self.undoblock = old_block

    # returns the width of the viewport's line number column in pango units
    def getLineNumberWidth(self):
        # find the maximum number of digits for a line number from all panes
        n = 0
        if self.prefs.getBool('display_show_line_numbers'):
            for pane in self.panes:
                n = max(n, len(str(pane.max_line_number)))
            # pad the total width by the width of a digit on either side
            n = (n + 2) * self.digit_width
        return n

    # returns the width of a string in pango units
    def getTextWidth(self, text):
        if len(text) == 0:
            return 0
        layout = self.create_pango_layout(text)
        layout.set_font_description(self.font)
        return layout.get_size()[0]

    # updates the size of the viewport
    # set 'compute_width' to False if the high water mark for line length can
    # be used to determine the required width for the viewport, use True for
    # this value otherwise
    def updateSize(self, compute_width, f=None):
        digit_width, stringWidth = self.digit_width, self.stringWidth
        string_width_cache = self.string_width_cache
        if compute_width:
            if f is None:
                panes = self.panes
            else:
                panes = [ self.panes[f] ]
            for f, pane in enumerate(panes):
                del pane.syntax_cache[:]
                del pane.diff_cache[:]
                # re-compute the high water mark
                pane.line_lengths = 0
                for line in pane.lines:
                    if line is not None:
                        line.compare_string = None
                        text = [ line.text ]
                        if line.is_modified:
                            text.append(line.modified_text)
                        for s in text:
                            if s is not None:
                                w = string_width_cache.get(s, None)
                                if w is None:
                                    string_width_cache[s] = w = stringWidth(s)
                                pane.line_lengths = max(pane.line_lengths, digit_width * w)
        # compute the maximum extents
        num_lines, line_lengths = 0, 0
        for pane in self.panes:
            num_lines = max(num_lines, len(pane.lines))
            line_lengths = max(line_lengths, pane.line_lengths)
        # account for any preedit text
        if self.im_preedit is not None:
            w = self._preedit_layout().get_size()[0]
            s = self.getLineText(self.current_pane, self.current_line)
            if s is not None:
                w += digit_width * stringWidth(s)
            line_lengths = max(line_lengths, w)
        # the cursor can move one line past the last line of text, add it so we
        # can scroll to see this line
        num_lines += 1
        width = self.getLineNumberWidth() + digit_width + line_lengths
        width = pixels(width)
        height = self.font_height * num_lines
        # update the adjustments
        self.hadj.upper = width
        self.hadj.step_increment = self.font_height
        self.vadj.upper = height
        self.vadj.step_increment = self.font_height

    # returns a line from the specified pane and offset
    def getLine(self, f, i):
        lines = self.panes[f].lines
        if i < len(lines):
            return lines[i]

    # returns the text for the specified line
    def getLineText(self, f, i):
        line = self.getLine(f, i)
        if line is not None:
            return line.getText()

    # Undo for changes to the cached line ending style
    class SetFormatUndo:
        def __init__(self, f, format, old_format):
            self.data = (f, format, old_format)

        def undo(self, viewer):
            f, format, old_format = self.data
            viewer.setFormat(f, old_format)

        def redo(self, viewer):
            f, format, old_format = self.data
            viewer.setFormat(f, format)

    # sets the cached line ending style
    def setFormat(self, f, format):
        pane = self.panes[f]
        if self.undoblock is not None:
            # create an Undo object for the action
            self.addUndo(FileDiffViewer.SetFormatUndo(f, format, pane.format))
        pane.format = format
        self.emit('format_changed', f, format)

    # Undo for the creation of Line objects
    class InstanceLineUndo:
        def __init__(self, f, i, reverse):
            self.data = (f, i, reverse)

        def undo(self, viewer):
            f, i, reverse = self.data
            viewer.instanceLine(f, i, not reverse)

        def redo(self, viewer):
            f, i, reverse = self.data
            viewer.instanceLine(f, i, reverse)

    # creates an instance of a Line object for the specified pane and offset
    # deletes an instance when 'reverse' is set to True
    def instanceLine(self, f, i, reverse=False):
        if self.undoblock is not None:
            # create an Undo object for the action
            self.addUndo(FileDiffViewer.InstanceLineUndo(f, i, reverse))
        pane = self.panes[f]
        if reverse:
            pane.lines[i] = None
        else:
            line = FileDiffViewer.Line()
            pane.lines[i] = line

    # Undo for changing the text for a Line object
    class UpdateLineTextUndo:
        def __init__(self, f, i, old_is_modified, old_text, is_modified, text):
            self.data = (f, i, old_is_modified, old_text, is_modified, text)

        def undo(self, viewer):
            f, i, old_is_modified, old_text, is_modified, text = self.data
            viewer.updateLineText(f, i, old_is_modified, old_text)

        def redo(self, viewer):
            f, i, old_is_modified, old_text, is_modified, text = self.data
            viewer.updateLineText(f, i, is_modified, text)

    def getMapFlags(self, f, i):
        flags = 0
        compare_text = self.getCompareString(f, i)
        if f > 0 and self.getCompareString(f - 1, i) != compare_text:
            flags |= 1
        if f + 1 < len(self.panes) and self.getCompareString(f + 1, i) != compare_text:
            flags |= 2
        line = self.getLine(f, i)
        if line is not None and line.is_modified:
            flags |= 4
        return flags

    # update line 'i' in pane 'f' to contain 'text'
    def updateLineText(self, f, i, is_modified, text):
        pane = self.panes[f]
        line = pane.lines[i]
        flags = self.getMapFlags(f, i)
        if self.undoblock is not None:
            # create an Undo object for the action
            self.addUndo(FileDiffViewer.UpdateLineTextUndo(f, i, line.is_modified, line.modified_text, is_modified, text))
        old_num_edits = pane.num_edits
        if is_modified and not line.is_modified:
            pane.num_edits += 1
        elif not is_modified and line.is_modified:
            pane.num_edits -= 1
        if pane.num_edits != old_num_edits:
            self.emit('num_edits_changed', f)
        line.is_modified = is_modified
        line.modified_text = text
        line.compare_string = None

        # update/invalidate all relevent caches and queue widgets for redraw
        if text is not None:
            pane.line_lengths = max(pane.line_lengths, self.digit_width * self.stringWidth(text))
        self.updateSize(False)

        y = int(self.vadj.get_value())
        h = self.font_height
        fs = []
        if f > 0:
            fs.append(f - 1)
        if f + 1 < len(self.panes):
            fs.append(f + 1)
        for fn in fs:
            otherpane = self.panes[fn]
            if i < len(otherpane.diff_cache):
                otherpane.diff_cache[i] = None
            self._queue_draw_lines(fn, i)
        if i < len(pane.syntax_cache):
            del pane.syntax_cache[i:]
        if i < len(pane.diff_cache):
            pane.diff_cache[i] = None
        self.dareas[f].queue_draw()
        if self.getMapFlags(f, i) != flags:
            self.map_cache = None
            self.map.queue_draw()

    # Undo for inserting a spacing line in a single pane
    class InsertNullUndo:
        def __init__(self, f, i, reverse):
            self.data = (f, i, reverse)

        def undo(self, viewer):
            f, i, reverse = self.data
            viewer.insertNull(f, i, not reverse)

        def redo(self, viewer):
            f, i, reverse = self.data
            viewer.insertNull(f, i, reverse)

    # insert a spacing line at line 'i' in pane 'f'
    # this caller must ensure the blocks and number of lines in each pane
    # are valid again
    def insertNull(self, f, i, reverse):
        if self.undoblock is not None:
            # create an Undo object for the action
            self.addUndo(FileDiffViewer.InsertNullUndo(f, i, reverse))
        pane = self.panes[f]
        lines = pane.lines
        # update/invalidate all relevent caches
        if reverse:
            del lines[i]
            if i < len(pane.syntax_cache):
                del pane.syntax_cache[i]
        else:
            lines.insert(i, None)
            if i < len(pane.syntax_cache):
                state = pane.syntax_cache[i][0]
                pane.syntax_cache.insert(i, [state, state, None, None])

    # Undo for manipulating a section of the line matching data
    class InvalidateLineMatchingUndo:
        def __init__(self, i, n, new_n):
            self.data = (i, n, new_n)

        def undo(self, viewer):
            i, n, new_n = self.data
            viewer.invalidateLineMatching(i, new_n, n)

        def redo(self, viewer):
            i, n, new_n = self.data
            viewer.invalidateLineMatching(i, n, new_n)

    # manipulate a section of the line matching data
    def invalidateLineMatching(self, i, n, new_n):
        if self.undoblock is not None:
            # create an Undo object for the action
            self.addUndo(FileDiffViewer.InvalidateLineMatchingUndo(i, n, new_n))
        # update/invalidate all relevent caches and queue widgets for redraw
        i2 = i + n
        for f, pane in enumerate(self.panes):
            if i < len(pane.diff_cache):
                if i2 + 1 < len(pane.diff_cache):
                    pane.diff_cache[i:i2] = new_n * [ None ]
                else:
                    del pane.diff_cache[i:]
            self.dareas[f].queue_draw()
        self.map_cache = None
        self.map.queue_draw()

    # Undo for alignment changes
    class AlignmentChangeUndo:
        def __init__(self, finished):
            self.data = finished

        def undo(self, viewer):
            finished = self.data
            viewer.alignmentChange(not finished)

        def redo(self, viewer):
            finished = self.data
            viewer.alignmentChange(finished)

    # update viewer in response to alignment changes
    def alignmentChange(self, finished):
        if self.undoblock is not None:
            # create an Undo object for the action
            self.addUndo(FileDiffViewer.AlignmentChangeUndo(finished))
        if finished:
            self.updateSize(False)

    # updates the alignment of 'n' lines starting from 'i'
    def updateAlignment(self, i, n, lines):
        self.alignmentChange(False)
        new_n = len(lines[0])
        i2 = i + n
        # insert spacing lines
        for f in range(len(self.panes)):
            for j in range(i2-1, i-1, -1):
                if self.getLine(f, j) is None:
                    self.insertNull(f, j, True)
            temp = lines[f]
            for j in range(new_n):
                if temp[j] is None:
                    self.insertNull(f, i + j, False)
        # update line matching for this block

        # FIXME: we should be able to do something more intelligent here...
        # the syntax cache will become invalidated.... we don't really need to
        # do that...
        self.invalidateLineMatching(i, n, new_n)
        self.alignmentChange(True)

    # Undo for changing how lines are cut into blocks for alignment
    class UpdateBlocksUndo:
        def __init__(self, old_blocks, blocks):
            self.data = (old_blocks, blocks)

        def undo(self, viewer):
            old_blocks, blocks = self.data
            viewer.updateBlocks(old_blocks)

        def redo(self, viewer):
            old_blocks, blocks = self.data
            viewer.updateBlocks(blocks)

    # change how lines are cut into blocks for alignment
    def updateBlocks(self, blocks):
        if self.undoblock is not None:
            # create an Undo object for the action
            self.addUndo(FileDiffViewer.UpdateBlocksUndo(self.blocks, blocks))
        self.blocks = blocks

    # insert 'n' blank lines in all panes
    def insertLines(self, i, n):
        # insert lines
        self.updateAlignment(i, 0, [ n * [ None ] for pane in self.panes ])
        pre, post = cutBlocks(i, self.blocks)
        pre.append(n)
        pre.extend(post)
        self.updateBlocks(pre)

        # update selection
        if self.current_line >= i:
            self.current_line += n
        if self.selection_line >= i:
            self.selection_line += n
        # queue redraws
        self.updateSize(False)
        self.map_cache = None
        self.map.queue_draw()

    # remove a line
    def removeSpacerLines(self, i, n, skip=-1):
        npanes, removed = len(self.panes), []
        for j in range(i, i + n):
            for f in range(npanes):
                line = self.getLine(f, j)
                if line is not None:
                    if line.line_number > 0 or (f != skip and line.getText() is not None):
                        break
            else:
                # remove line
                for f in range(npanes):
                    line = self.getLine(f, j)
                    if line is not None:
                        # create undo to record any edits to the line before
                        # removing it
                        self.updateLineText(f, j, False, None)
                        # remove the line so it doesn't persist as a spacer
                        self.instanceLine(f, j, True)
                removed.append(j)

        nremoved = len(removed)
        if nremoved > 0:
            # update blocks
            bi, bii, blocks = 0, 0, self.blocks[:]
            for j in removed:
                while bii + blocks[bi] < j:
                    bii += blocks[bi]
                    bi += 1
                if blocks[bi] == 1:
                    del blocks[bi]
                else:
                    blocks[bi] -= 1
                bii += 1
            self.updateBlocks(blocks)

            self.alignmentChange(False)
            removed.reverse()
            for j in removed:
                for f in range(npanes):
                    self.insertNull(f, j, True)
            # FIXME: we should be able to do something more intelligent here...
            # the syntax cache will become invalidated.... we don't really need
            # to do that...
            self.invalidateLineMatching(i, n, n - nremoved)
            self.alignmentChange(True)

            # queue redraws
            self.updateSize(False)
            self.map_cache = None
            self.map.queue_draw()
        return nremoved

    # Undo for replacing the lines for a single pane with a new set
    class ReplaceLinesUndo:
        def __init__(self, f, lines, new_lines, max_num, new_max_num):
            self.data = (f, lines, new_lines, max_num, new_max_num)

        def undo(self, viewer):
            f, lines, new_lines, max_num, new_max_num = self.data
            viewer.replaceLines(f, new_lines, lines, new_max_num, max_num)

        def redo(self, viewer):
            f, lines, new_lines, max_num, new_max_num = self.data
            viewer.replaceLines(f, lines, new_lines, max_num, new_max_num)

    # replace the lines for a single pane with a new set
    def replaceLines(self, f, lines, new_lines, max_num, new_max_num):
        if self.undoblock is not None:
            # create an Undo object for the action
            self.addUndo(FileDiffViewer.ReplaceLinesUndo(f, lines, new_lines, max_num, new_max_num))
        pane = self.panes[f]
        pane.lines = new_lines
        # update/invalidate all relevent caches and queue widgets for redraw
        old_num_edits = pane.num_edits
        pane.num_edits = 0
        for line in new_lines:
            if line is not None and line.is_modified:
                pane.num_edits += 1
        if pane.num_edits != old_num_edits:
            self.emit('num_edits_changed', f)
        del pane.syntax_cache[:]
        pane.max_line_number = new_max_num
        self.dareas[f].queue_draw()
        self.updateSize(True, f)
        self.map_cache = None
        self.map.queue_draw()

    # create a hash for a line to use for line matching
    def _alignmentHash(self, line):
        text = line.getText()
        if text is None:
            return None
        pref = self.prefs.getBool
        if pref('align_ignore_whitespace'):
            # strip all white space from the string
            for c in whitespace:
                text = text.replace(c, '')
        else:
            # hashes for non-null lines should start with '+' to distinguish
            # them from blank lines
            if pref('align_ignore_endofline'):
                text = strip_eol(text)
            if pref('align_ignore_blanklines') and isBlank(text):
                # consider all lines containing only white space as the same
                return ''

            if pref('align_ignore_whitespace_changes'):
                # replace all blocks of white space with a single space
                pc = True
                r = []
                append = r.append
                for c in text:
                    if c in whitespace:
                        if pc:
                            append(' ')
                            pc = False
                    else:
                        append(c)
                        pc = True
                text = ''.join(r)
        if pref('align_ignore_case'):
            # convert everything to upper case
            text = text.upper()
        return text

    # align sets of lines by inserting null spacers and updating the size
    # of blocks to which they belong
    #
    # Leftlines and rightlines are list of list of lines.  Only the inner list
    # of lines are aligned (leftlines[-1] and rightlines[0]). Any spacers
    # needed for alignment are inserted in all lists of lines for a particular
    # side to keep them all in sync.
    def alignBlocks(self, leftblocks, leftlines, rightblocks, rightlines):
        blocks = ( leftblocks, rightblocks )
        lines = ( leftlines, rightlines )
        # get the inner lines we are to match
        middle = ( leftlines[-1], rightlines[0] )
        # eliminate any existing spacer lines
        mlines = ( [ line for line in middle[0] if line is not None ],
                   [ line for line in middle[1] if line is not None ] )
        s1, s2 = mlines
        n1, n2 = 0, 0
        # hash lines according to the alignment preferences
        a = self._alignmentHash
        t1 = [ a(s) for s in s1 ]
        t2 = [ a(s) for s in s2 ]
        # align s1 and s2 by inserting spacer lines
        # this will be used to determine which lines from the inner lists of
        # lines should be neighbours
        for block in patience_diff(t1, t2):
            delta = (n1 + block[0]) - (n2 + block[1])
            if delta < 0:
                # insert spacer lines in s1
                i = n1 + block[0]
                s1[i:i] = -delta * [ None ]
                n1 -= delta
            elif delta > 0:
                # insert spacer lines in s2
                i = n2 + block[1]
                s2[i:i] = delta * [ None ]
                n2 += delta
        nmatch = len(s1)

        # insert spacer lines in leftlines and rightlines and increase the
        # size of blocks in leftblocks and rightblocks as spacer lines are
        # inserted
        #
        # advance one row at a time inserting spacer lines as we go
        # 'i' indicates which row we are processing
        # 'k' indicates which pair of neighbours we are processing
        i, k = 0, 0
        bi = [ 0, 0 ]
        bn = [ 0, 0 ]
        while True:
            # if we have reached the end of the list for any side, it needs
            # spacer lines to align with the other side
            insert = [ i >= len(m) for m in middle ]
            if insert == [ True, True ]:
                # we have reached the end of both inner lists of lines
                # we are done
                break
            if insert == [ False, False ] and k < nmatch:
                # determine if either side needs spacer lines to make the
                # inner list of lines match up
                accept = True
                for j in range(2):
                    m = mlines[j][k]
                    if middle[j][i] is not m:
                        # this line does not correspond to the pair of
                        # neighbours we expected
                        if m is None:
                            # we expected to find a null here so insert one
                            insert[j] = True
                        else:
                            # we have a null but didn't expect one we will not
                            # obtain the pairing we expected by iserting nulls
                            accept = False
                if accept:
                    # our lines will be correctly paired up
                    # move on to the next pair
                    k += 1
                else:
                    # insert spacer lines as needed
                    insert = [ m[i] is not None for m in middle ]
            for j in range(2):
                if insert[j]:
                    # insert spacers lines for side 'j'
                    for temp in lines[j]:
                        temp.insert(i, None)
                    blocksj = blocks[j]
                    bij = bi[j]
                    bnj = bn[j]
                    # append a new block if needed
                    if len(blocksj) == 0:
                        blocksj.append(0)
                    # advance to the current block
                    while bnj + blocksj[bij] < i:
                        bnj += blocksj[bij]
                        bij += 1
                    # increase the current block size
                    blocksj[bij] += 1
            # advance to the next row
            i += 1

    # replace the contents of pane 'f' with the strings list of strings 'ss'
    def replaceContents(self, f, ss):
        self.alignmentChange(False)
        # determine the format for the text
        self.setFormat(f, getFormat(ss))

        # create an initial set of blocks for the lines
        blocks = []
        n = len(ss)
        if n > 0:
            blocks.append(n)
        # create line objects for the text
        Line = FileDiffViewer.Line
        mid = [ [ Line(j + 1, ss[j]) for j in range(n) ] ]

        if f > 0:
            # align with panes to the left
            # use copies so the originals can be used by the Undo object
            leftblocks = self.blocks[:]
            leftlines = [ pane.lines[:] for pane in self.panes[:f] ]
            removeNullLines(leftblocks, leftlines)
            self.alignBlocks(leftblocks, leftlines, blocks, mid)
            mid[:0] = leftlines
            blocks = mergeBlocks(leftblocks, blocks)
        if f + 1 < len(self.panes):
            # align with panes to the right
            # use copies so the originals can be used by the Undo object
            rightblocks = self.blocks[:]
            rightlines = [ pane.lines[:] for pane in self.panes[f + 1:] ]
            removeNullLines(rightblocks, rightlines)
            self.alignBlocks(blocks, mid, rightblocks, rightlines)
            mid.extend(rightlines)
            blocks = mergeBlocks(blocks, rightblocks)

        # update the lines for this pane
        pane = self.panes[f]
        old_n = len(pane.lines)
        new_n = len(mid[f])
        self.replaceLines(f, pane.lines, mid[f], pane.max_line_number, n)

        # insert or remove spacer lines from the other panes
        insertNull, getLine = self.insertNull, self.getLine
        for f_idx in range(len(self.panes)):
            if f_idx != f:
                for j in range(old_n-1, -1, -1):
                    if getLine(f_idx, j) is None:
                        insertNull(f_idx, j, True)
                temp = mid[f_idx]
                for j in range(new_n):
                    if temp[j] is None:
                        insertNull(f_idx, j, False)

        # update the blocks
        self.invalidateLineMatching(0, old_n, new_n)
        self.updateBlocks(blocks)
        self.alignmentChange(True)
        # update cursor
        self.setLineMode()
        self.setCurrentLine(self.current_pane, min(self.current_line, len(pane.lines) + 1))

    # refresh the lines to contain new objects with updated line numbers and
    # no local edits
    def bakeEdits(self, f):
        pane, lines, line_num = self.panes[f], [], 0
        for i in range(len(pane.lines)):
            s = self.getLineText(f, i)
            if s is None:
                lines.append(None)
            else:
                line_num += 1
                lines.append(FileDiffViewer.Line(line_num, s))

        # update loaded pane
        self.replaceLines(f, pane.lines, lines, pane.max_line_number, line_num)

    # update the contents for a line, creating the line if necessary
    def updateText(self, f, i, text, is_modified=True):
        if self.panes[f].lines[i] is None:
            self.instanceLine(f, i)
        self.updateLineText(f, i, is_modified, text)

    # replace the current selection with 'text'
    def replaceText(self, text):
        # record the edit mode as we will be updating the selection too
        self.recordEditMode()

        # find the extents of the current selection
        f = self.current_pane
        pane = self.panes[f]
        nlines = len(pane.lines)
        line0, line1 = self.selection_line, self.current_line
        if self.mode == LINE_MODE:
            col0, col1 = 0, 0
            if line1 < line0:
                line0, line1 = line1, line0
            if line1 < nlines:
                line1 += 1
        else:
            col0, col1 = self.selection_char, self.current_char
            if line1 < line0 or (line1 == line0 and col1 < col0):
                line0, col0, line1, col1 = line1, col1, line0, col0

        # update text
        if text is None:
            text = ''
        # split the replacement text into lines
        ss = splitlines(text)
        if len(ss) == 0 or len(ss[-1]) != len_minus_line_ending(ss[-1]):
            ss.append('')
        # change the format to that of the target pane
        if pane.format == 0:
            self.setFormat(f, getFormat(ss))
        ss = [ convert_to_format(s, pane.format) for s in ss ]
        # prepend original text that was before the selection
        if col0 > 0:
            pre = self.getLineText(f, line0)[:col0]
            ss[0] = pre + ss[0]
        # remove the last line as it needs special casing
        lastcol = 0
        if len(ss) > 0:
            last = ss[-1]
            if len(last) == len_minus_line_ending(last):
                del ss[-1]
                lastcol = len(last)
        cur_line = line0 + len(ss)
        if lastcol > 0:
            # the replacement text does not end with a new line character
            # we need more text to finish the line, search forward for some
            # more text
            while line1 < nlines:
                s = self.getLineText(f, line1)
                line1 += 1
                if s is not None:
                    last = last + s[col1:]
                    break
                col1 = 0
            ss.append(last)
        elif col1 > 0:
            # append original text that was after the selection
            s = self.getLineText(f, line1)
            ss.append(s[col1:])
            line1 += 1
        # insert blank lines for more space if needed
        npanes = len(self.panes)

        # remove blank rows if possible
        n_need = len(ss)
        n_have = line1 - line0
        n_have -= self.removeSpacerLines(line0, n_have, f)
        delta = n_have - n_need
        if delta < 0:
            self.insertLines(line0 + n_have, -delta)
            delta = 0
        # update the text
        for i, s in enumerate(ss):
            self.updateText(f, line0 + i, s)
        # clear all unused lines
        for i in range(delta):
            self.updateText(f, line0 + n_need + i, None)
        # update selection
        if self.mode == LINE_MODE:
            self.setCurrentLine(f, line0 + max(n_need, 1) - 1, line0)
        else:
            self.setCurrentChar(cur_line, lastcol)
        self.recordEditMode()

    # manually adjust line matching so 'line1' of pane 'f' is a neighbour of
    # 'line2' from pane 'f+1'
    def align(self, f, line1, line2):
        # record the edit mode as we will be updating the selection too
        self.recordEditMode()

        # find the smallest span of blocks that inclues line1 and line2
        start = line1
        end = line2
        if end < start:
            start, end = end, start
        pre_blocks = []
        mid = []
        post_blocks = []
        n = 0
        for b in self.blocks:
            if n + b <= start:
                dst = pre_blocks
            elif n <= end:
                dst = mid
            else:
                dst = post_blocks
            dst.append(b)
            n += b
        start = sum(pre_blocks)
        end = start + sum(mid)

        # cut the span of blocks into three sections:
        # 1. lines before the matched pair
        # 2. the matched pair
        # 3. lines after the matched pair
        # each section has lines and blocks for left and right sides
        lines_s = [ [], [], [] ]
        cutblocks = [ [], [], [] ]
        lines = [ pane.lines for pane in self.panes ]
        nlines = len(lines[0])
        for temp, m in zip([ lines[:f + 1], lines[f + 1:] ], [ line1, line2 ]):
            # cut the blocks just before the line being matched
            pre, post = cutBlocks(m - start, mid)
            if len(temp) == 1:
                # if we only have one pane on this side, we don't need to
                # preserve other cuts
                pre = createBlock(sum(pre))
            # the first section of lines to match
            lines_s[0].append([ s[start:m] for s in temp ])
            cutblocks[0].append(pre)
            # the line to match may be after the actual lines
            if m < nlines:
                m1 = [ [ s[m] ] for s in temp ]
                m2 = [ s[m + 1:end] for s in temp ]
                # cut the blocks just after the line being matched
                b1, b2 = cutBlocks(1, post)
                if len(temp) == 1:
                    # if we only have one pane on this side, we don't need to
                    # preserve other cuts
                    b2 = createBlock(sum(b2))
            else:
                m1 = [ [] for s in temp ]
                m2 = [ [] for s in temp ]
                b1, b2 = [], []
            # the second section of lines to match
            lines_s[1].append(m1)
            cutblocks[1].append(b1)
            # the third section of lines to match
            lines_s[2].append(m2)
            cutblocks[2].append(b2)

        # align each section and concatenate the results
        finallines = [ [] for s in lines ]
        for b, lines_t in zip(cutblocks, lines_s):
            removeNullLines(b[0], lines_t[0])
            removeNullLines(b[1], lines_t[1])
            self.alignBlocks(b[0], lines_t[0], b[1], lines_t[1])
            temp = lines_t[0]
            temp.extend(lines_t[1])
            for dst, s in zip(finallines, temp):
                dst.extend(s)
            pre_blocks.extend(mergeBlocks(b[0], b[1]))
        pre_blocks.extend(post_blocks)

        # update the actual lines and blocks
        self.updateAlignment(start, end - start, finallines)
        self.updateBlocks(pre_blocks)

        i = len(lines_s[0][0][0])
        self.removeSpacerLines(start + i, len(finallines[0]) - i)
        i -= min(self.removeSpacerLines(start, i), i - 1)

        # update selection
        self.setCurrentLine(self.current_pane, start + i)
        self.recordEditMode()

    # Undo for changing the selection mode and range
    class EditModeUndo:
        def __init__(self, mode, current_pane, current_line, current_char, selection_line, selection_char, cursor_column):
            self.data = (mode, current_pane, current_line, current_char, selection_line, selection_char, cursor_column)

        def undo(self, viewer):
            mode, current_pane, current_line, current_char, selection_line, selection_char, cursor_column = self.data
            viewer.setEditMode(mode, current_pane, current_line, current_char, selection_line, selection_char, cursor_column)

        def redo(self, viewer):
            self.undo(viewer)

    # appends an undo to reset to the specified selection mode and range
    # this should be called before and after actions that also change the
    # selection
    def recordEditMode(self):
        if self.undoblock is not None:
            self.addUndo(FileDiffViewer.EditModeUndo(self.mode, self.current_pane, self.current_line, self.current_char, self.selection_line, self.selection_char, self.cursor_column))

    # change the selection mode
    def setEditMode(self, mode, f, current_line, current_char, selection_line, selection_char, cursor_column):
        old_f = self.current_pane
        self.mode = mode
        self.current_pane = f
        self.current_line = current_line
        self.current_char = current_char
        self.selection_line = selection_line
        self.selection_char = selection_char
        self.cursor_column = cursor_column
        if mode == CHAR_MODE:
            self.setCurrentChar(self.current_line, self.current_char, True)
        else:
            self.setCurrentLine(self.current_pane, self.current_line, self.selection_line)
        self.emit('cursor_changed')
        self.emit('mode_changed')
        # queue a redraw to show the updated selection
        self.dareas[old_f].queue_draw()

    # queue a range of lines for redrawing
    def _queue_draw_lines(self, f, line0, line1=None):
        if line1 is None:
            line1 = line0
        elif line0 > line1:
            line0, line1 = line1, line0
        darea = self.dareas[f]
        w, h = darea.get_allocation().width, self.font_height
        darea.queue_draw_area(0, line0 * h - int(self.vadj.get_value()), w, (line1 - line0 + 1) * h)

    # scroll vertically to ensure the current line is visible
    def _ensure_line_is_visible(self, i):
        h = self.font_height
        lower = i * h
        upper = lower + h
        vadj = self.vadj
        v = vadj.get_value()
        ps = vadj.page_size
        if lower < v:
            vadj.set_value(lower)
        elif upper > v + ps:
            vadj.set_value(upper - ps)

    # change the current selection in LINE_MODE
    # use extend=True to extend the selection
    def setCurrentLine(self, f, i, selection=None):
        # remember old cursor position so we can just redraw what is necessary
        old_f = self.current_pane
        line0, line1 = self.current_line, self.selection_line

        # clamp input values
        f = max(min(f, len(self.panes) - 1), 0)
        i = max(min(i, len(self.panes[f].lines)), 0)

        # update cursor
        self.current_pane = f
        self.current_line = i
        if selection is None:
            self.selection_line = i
        else:
            self.selection_line = selection

        self.emit('cursor_changed')

        # invalidate old selection area
        self._queue_draw_lines(old_f, line0, line1)
        # invalidate new selection area
        self._queue_draw_lines(f, i, self.selection_line)

        # ensure the new cursor position is visible
        self._ensure_line_is_visible(i)

    # returns True if the line has preedit text
    def hasPreedit(self, f, i):
        return self.mode == CHAR_MODE and self.current_pane == f and self.current_line == i and self.im_preedit is not None

    # create a layout for the existing preedit text
    def _preedit_layout(self, partial=False):
        s, a, c = self.im_preedit
        if partial:
            s = s[:c]
        layout = self.create_pango_layout(s)
        layout.set_font_description(self.font)
        layout.set_attributes(a)
        return layout

    # inform input method about cursor motion
    def _cursor_position_changed(self, recompute):
        if self.mode == CHAR_MODE:
            # update input method
            h = self.font_height
            if recompute:
                self.cursor_pos = (pixels(self._get_cursor_x_offset()), self.current_line * h)
            x, y = self.cursor_pos
            x -= int(self.hadj.get_value())
            y -= int(self.vadj.get_value())
            # translate to a position relative to the window
            x, y = self.dareas[self.current_pane].translate_coordinates(self.get_toplevel(), x, y)
            # input methods support widgets are centred horizontally about the
            # cursor, a width of 50 seems to give a better widget positions
            self.im_context.set_cursor_location((x, y, 50, h))

    # get the position of the cursor in pango units
    def _get_cursor_x_offset(self):
        j = self.current_char
        if j > 0:
            text = self.getLineText(self.current_pane, self.current_line)[:j]
            return self.getTextWidth(''.join(self.expand(text)))
        return 0

    # scroll to ensure the current cursor position is visible
    def _ensure_cursor_is_visible(self):
        f, i, j = self.current_pane, self.current_line, self.current_char
        h = self.font_height

        # find the cursor's horizontal range
        lower = self._get_cursor_x_offset()
        if self.im_preedit is not None:
            lower += self._preedit_layout(True).get_size()[0]
        upper = lower + self.getLineNumberWidth() + self.digit_width
        lower, upper = pixels(lower), pixels(upper)

        # scroll horizontally
        hadj = self.hadj
        v = hadj.get_value()
        ps = hadj.page_size
        if lower < v:
            hadj.set_value(lower)
        elif upper > v + ps:
            hadj.set_value(upper - ps)

        # scroll vertically to current line
        self._ensure_line_is_visible(i)

    def __set_clipboard_text(self, clipboard, s):
        # remove embedded nulls as the clipboard cannot handle them
        gtk.clipboard_get(clipboard).set_text(s.replace('\0', ''))

    # change the current selection in CHAR_MODE
    # use extend=True to extend the selection
    def setCurrentChar(self, i, j, si=None, sj=None):
        f = self.current_pane

        # remember old cursor position so we can just redraw what is necessary
        line0, line1 = self.current_line, self.selection_line

        # clear remembered cursor column
        self.cursor_column = -1
        # update cursor and selection
        extend = (si is not None and sj is not None)
        if not extend:
            si, sj = i, j
        self.current_line = i
        self.current_char = j
        self.selection_line = si
        self.selection_char = sj

        if extend:
            self.__set_clipboard_text(gtk.gdk.SELECTION_PRIMARY, self.getSelectedText())

        self._cursor_position_changed(True)
        self.emit('cursor_changed')

        # invalidate old selection area
        self._queue_draw_lines(f, line0, line1)
        # invalidate new selection area
        self._queue_draw_lines(f, i, self.selection_line)

        # ensure the new cursor position is visible
        self._ensure_cursor_is_visible()

    # returns the currently selected text
    def getSelectedText(self):
        f = self.current_pane
        start, end = self.selection_line, self.current_line
        # find extents of selection
        if self.mode == LINE_MODE:
            if end < start:
                start, end = end, start
            end += 1
            col0, col1 = 0, 0
        else:
            col0, col1 = self.selection_char, self.current_char
            if end < start or (end == start and col1 < col0):
                start, col0, end, col1 = end, col1, start, col0
            if col1 > 0:
               end += 1
        # get the text for the selected lines
        end = min(end, len(self.panes[f].lines))
        ss = [ self.getLineText(f, i) for i in range(start, end) ]
        # trim out the unselected parts of the lines
        # check for col > 0 as some lines may be null
        if col1 > 0:
            ss[-1] = ss[-1][:col1]
        if col0 > 0:
            ss[0] = ss[0][col0:]
        return ''.join([ s for s in ss if s is not None ])

    # expands the selection to include everything
    def select_all(self):
        if self.mode == LINE_MODE or self.mode == CHAR_MODE:
            f = self.current_pane
            self.selection_line = 0
            self.current_line = len(self.panes[f].lines)
            if self.mode == CHAR_MODE:
                self.selection_char = 0
                self.current_char = 0
            self.dareas[f].queue_draw()

    # returns the index of the last character in text that should be left of
    # 'x' pixels from the edge of the darea widget
    # if partial=True, include characters only partially to the left of 'x'
    def _getPickedCharacter(self, text, x, partial):
        if text is None:
            return 0
        n = len(text)
        w = self.getLineNumberWidth()
        for i, s in enumerate(self.expand(text)):
            width = self.getTextWidth(s)
            tmp = w
            if partial:
                tmp += width // 2
            else:
                tmp += width
            if x < pixels(tmp):
                return i
            w += width
        return n

    # update the selection in response to a mouse button press
    def button_press(self, f, x, y, extend):
        if y < 0:
            x, y = -1, 0
        i = min(y // self.font_height, len(self.panes[f].lines))
        if self.mode == CHAR_MODE and f == self.current_pane:
            text = strip_eol(self.getLineText(f, i))
            j = self._getPickedCharacter(text, x, True)
            if extend:
                si, sj = self.selection_line, self.selection_char
            else:
                si, sj = None, None
            self.setCurrentChar(i, j, si, sj)
        else:
            if self.mode == ALIGN_MODE:
                extend = False
            elif self.mode == CHAR_MODE:
                self.setLineMode()
            if extend and f == self.current_pane:
                selection = self.selection_line
            else:
                selection = None
            self.setCurrentLine(f, i, selection)

    # callback for mouse button presses in the text window
    def darea_button_press_cb(self, widget, event, f):
        self.get_toplevel().set_focus(self)
        x = int(event.x + self.hadj.get_value())
        y = int(event.y + self.vadj.get_value())
        nlines = len(self.panes[f].lines)
        i = min(y // self.font_height, nlines)
        if event.button == 1:
            # left mouse button
            if event.type == gtk.gdk._2BUTTON_PRESS:
                # double click
                if self.mode == ALIGN_MODE:
                    self.setLineMode()
                if self.mode == LINE_MODE:
                    # change to CHAR_MODE
                    self.setCurrentLine(f, i)
                    # silently switch mode so the viewer does not scroll yet.
                    self.mode = CHAR_MODE
                    self._im_focus_in()
                    self.button_press(f, x, y, False)
                    self.emit('mode_changed')
                elif self.mode == CHAR_MODE and self.current_pane == f:
                    # select word
                    text = strip_eol(self.getLineText(f, i))
                    if text is not None:
                        n = len(text)
                        j = self._getPickedCharacter(text, x, False)
                        if j < n:
                            ss = self.expand(text[:n])
                            c = getCharacterClass(text[j])
                            k = j
                            while k > 0 and getCharacterClass(text[k - 1]) == c:
                                k -= 1
                            while j < n and getCharacterClass(text[j]) == c:
                                j += 1
                            self.setCurrentChar(i, j, i, k)
            elif event.type == gtk.gdk._3BUTTON_PRESS:
                # triple click, select a whole line
                if self.mode == CHAR_MODE and self.current_pane == f:
                    i2 = min(i + 1, nlines)
                    self.setCurrentChar(i2, 0, i, 0)
            else:
                # update the selection
                is_shifted = event.state & gtk.gdk.SHIFT_MASK
                extend = (is_shifted and f == self.current_pane)
                self.button_press(f, x, y, extend)
        elif event.button == 2:
            # middle mouse button, paste primary selection
            if self.mode == CHAR_MODE and f == self.current_pane:
                self.button_press(f, x, y, False)
                self.openUndoBlock()
                gtk.clipboard_get(gtk.gdk.SELECTION_PRIMARY).request_text(self.receive_clipboard_text_cb)
                self.closeUndoBlock()
        elif event.button == 3:
            # right mouse button, raise context sensitive menu
            can_align = (self.mode == LINE_MODE and (f == self.current_pane + 1 or f == self.current_pane - 1))
            can_isolate = (self.mode == LINE_MODE and f == self.current_pane)
            can_merge = (self.mode == LINE_MODE and f != self.current_pane)
            can_select = ((self.mode == LINE_MODE or self.mode == CHAR_MODE) and f == self.current_pane)
            can_swap = (f != self.current_pane)

            menu = createMenu(
                      [ [_('Align with Selection'), self.align_with_selection_cb, [f, i], gtk.STOCK_EXECUTE, None, can_align],
                      [_('Isolate'), self.button_cb, 'isolate', None, None, can_isolate ],
                      [_('Merge Selection'), self.merge_lines_cb, f, None, None, can_merge],
                      [],
                      [_('Cut'), self.button_cb, 'cut', gtk.STOCK_CUT, None, can_select],
                      [_('Copy'), self.button_cb, 'copy', gtk.STOCK_COPY, None, can_select],
                      [_('Paste'), self.button_cb, 'paste', gtk.STOCK_PASTE, None, can_select],
                      [],
                      [_('Select All'), self.button_cb, 'select_all', None, None, can_select],
                      [_('Clear Edits'), self.button_cb, 'clear_edits', gtk.STOCK_CLEAR, None, can_isolate],
                      [],
                      [_('Swap with Selected Pane'), self.swap_panes_cb, f, None, None, can_swap] ])
            menu.popup(None, None, None, event.button, event.time)

    # callback used to notify us about click and drag motion
    def darea_motion_notify_cb(self, widget, event, f):
        if event.state & gtk.gdk.BUTTON1_MASK:
            # left mouse button
            extend = (f == self.current_pane)
            x = int(event.x + self.hadj.get_value())
            y = int(event.y + self.vadj.get_value())
            self.button_press(f, x, y, extend)

    # return a list of (begin, end, flag) tuples marking characters that differ
    # from the text in line 'i' from panes 'f' and 'f+1'
    # return the results for pane 'f' if idx=0 and 'f+1' if idx=1
    def getDiffRanges(self, f, i, idx, flag):
        result = []
        s1 = nullToEmpty(self.getLineText(f, i))
        s2 = nullToEmpty(self.getLineText(f + 1, i))

        # ignore blank lines if specified
        if self.prefs.getBool('display_ignore_blanklines') and isBlank(s1) and isBlank(s2):
            return result

        # ignore white space preferences
        ignore_whitespace = self.prefs.getBool('display_ignore_whitespace')
        if ignore_whitespace or self.prefs.getBool('display_ignore_whitespace_changes'):
            if idx == 0:
                s = s1
            else:
                s = s2
            if self.prefs.getBool('display_ignore_endofline'):
                s = strip_eol(s)

            s1 = nullToEmpty(self.getCompareString(f, i))
            s2 = nullToEmpty(self.getCompareString(f + 1, i))

            # build a mapping from characters in compare string to those in the
            # original string
            v = 0
            lookup = []
            # we only need to consider white space here as those are the only
            # ones that can change the number of characters in the compare
            # string
            if ignore_whitespace:
                # all white space characters were removed
                for c in s:
                    if c not in whitespace:
                        lookup.append(v)
                    v += 1
            else:
                # all white space characters were replaced with a single space
                first = True
                for c in s:
                    if c in whitespace:
                        # only include the first white space character of a span
                        if first:
                            lookup.append(v)
                            first = False
                    else:
                        lookup.append(v)
                        first = True
                    v += 1
            lookup.append(v)
        else:
            lookup = None

        start = 0
        for block in difflib.SequenceMatcher(None, s1, s2).get_matching_blocks():
            end = block[idx]
            # skip zero length blocks
            if start < end:
                if lookup is None:
                    result.append((start, end, flag))
                else:
                    # map to indices for the original string
                    lookup_start = lookup[start]
                    lookup_end = lookup[end]
                    # scan for whitespace and skip those sections if specified
                    for j in range(lookup_start, lookup_end):
                        if ignore_whitespace and s[j] in whitespace:
                            if lookup_start != j:
                                result.append((lookup_start, j, flag))
                            lookup_start = j + 1
                    if lookup_start != lookup_end:
                        result.append((lookup_start, lookup_end, flag))
            start = end + block[2]
        return result

    # returns a hash of a string that can be used to quickly compare strings
    # according to the display preferences
    def getCompareString(self, f, i):
        line = self.getLine(f, i)
        if line is None:
            return None
        # if a cached value exists, use it
        s = line.compare_string
        if s is not None:
            return s
        # compute a new hash and cache it
        s = line.getText()
        if s is not None:
            if self.prefs.getBool('display_ignore_endofline'):
                s = strip_eol(s)
            if self.prefs.getBool('display_ignore_blanklines') and isBlank(s):
                return None
            if self.prefs.getBool('display_ignore_whitespace'):
                # strip all white space characters
                for c in whitespace:
                    s = s.replace(c, '')
            elif self.prefs.getBool('display_ignore_whitespace_changes'):
                # map all spans of white space characters to a single space
                first = True
                temp = []
                for c in s:
                    if c in whitespace:
                        if first:
                            temp.append(' ')
                            first = False
                    else:
                        temp.append(c)
                        first = True
                s = ''.join(temp)
            if self.prefs.getBool('display_ignore_case'):
                # force everything to be upper case
                s = s.upper()
            # cache the hash
            line.compare_string = s
        return s

    # draw the text viewport
    def darea_expose_cb(self, widget, event, f):
        pane = self.panes[f]
        syntax = theResources.getSyntax(self.syntax)

        offset_x, offset_y, width, height = event.area
        x = offset_x + int(self.hadj.get_value())
        y = offset_y + int(self.vadj.get_value())

        # draw to a pixmap to avoid screen flicker
        pixmap = gtk.gdk.Pixmap(widget.window, width, height)

        cr = pixmap.cairo_create()
        cr.translate(-x, -y)

        maxx = x + width
        maxy = y + height
        line_number_width = pixels(self.getLineNumberWidth())
        h = self.font_height

        diffcolours = [ theResources.getDifferenceColour(f), theResources.getDifferenceColour(f + 1) ]
        diffcolours.append((diffcolours[0] + diffcolours[1]) * 0.5)

        # iterate over each exposed line
        i = y // h
        y_start = i * h
        while y_start < maxy:
            line = self.getLine(f, i)

            # line numbers
            if line_number_width > 0 and 0 < maxx and line_number_width > x:
                cr.save()
                cr.rectangle(0, y_start, line_number_width, h)
                cr.clip()
                colour = theResources.getColour('line_number_background')
                cr.set_source_rgb(colour.red, colour.green, colour.blue)
                cr.paint()

                ## draw the line number
                if line is not None and line.line_number > 0:
                    colour = theResources.getColour('line_number')
                    cr.set_source_rgb(colour.red, colour.green, colour.blue)
                    layout = self.create_pango_layout(str(line.line_number))
                    layout.set_font_description(self.font)
                    w = pixels(layout.get_size()[0] + self.digit_width)
                    cr.move_to(line_number_width - w, y_start)
                    cr.show_layout(layout)
                cr.restore()

            x_start = line_number_width
            if x_start < maxx:
                cr.save()
                cr.rectangle(x_start, y_start, maxx - x_start, h)
                cr.clip()

                text = self.getLineText(f, i)
                ss = None

                # enlarge cache to fit pan.diff_cache[i]
                if i >= len(pane.diff_cache):
                    pane.diff_cache.extend((i - len(pane.diff_cache) + 1) * [ None ])
                # construct a list of ranges for this lines character
                # differences if not already cached
                if pane.diff_cache[i] is None:
                    flags = 0
                    temp_diff = []
                    comptext = self.getCompareString(f, i)
                    if f > 0:
                        # compare with neighbour to the left
                        if self.getCompareString(f - 1, i) != comptext:
                            flags |= 1
                            if text is not None:
                                temp_diff = mergeRanges(temp_diff, self.getDiffRanges(f - 1, i, 1, 1))
                    if f + 1 < len(self.panes):
                        # compare with neighbour to the right
                        if self.getCompareString(f + 1, i) != comptext:
                            flags |= 2
                            if text is not None:
                                temp_diff = mergeRanges(temp_diff, self.getDiffRanges(f, i, 0, 2))

                    chardiff = []
                    if text is not None:
                        # expand text into a list of visual representations
                        ss = self.expand(text)

                        # find the size of each region in pango units
                        old_end = 0
                        x_temp = 0
                        for start, end, tflags in temp_diff:
                            layout = self.create_pango_layout(''.join(ss[old_end:start]))
                            layout.set_font_description(self.font)
                            x_temp += layout.get_size()[0]
                            layout = self.create_pango_layout(''.join(ss[start:end]))
                            layout.set_font_description(self.font)
                            w = layout.get_size()[0]
                            chardiff.append((start, end, x_temp, w, diffcolours[tflags - 1]))
                            old_end = end
                            x_temp += w
                    # cache flags and character diff ranges
                    pane.diff_cache[i] = (flags, chardiff)
                else:
                    flags, chardiff = pane.diff_cache[i]

                # account for preedit changes
                if f > 0 and self.hasPreedit(f - 1, i):
                    flags |= 1
                if f + 1 < len(self.panes) and self.hasPreedit(f + 1, i):
                    flags |= 2
                has_preedit = self.hasPreedit(f, i)
                if has_preedit:
                    # we have preedit text
                    preeditlayout = self._preedit_layout()
                    preeditwidth = preeditlayout.get_size()[0]
                    if f > 0:
                        flags |= 1
                    if f + 1 < len(self.panes):
                        flags |= 2
                else:
                    preeditwidth = 0
                # draw background
                colour = theResources.getColour('text_background')
                alpha = theResources.getFloat('character_difference_opacity')
                if flags != 0:
                    colour = (diffcolours[flags - 1] * theResources.getFloat('line_difference_opacity')).over(colour)
                cr.set_source_rgb(colour.red, colour.green, colour.blue)
                cr.paint()

                # make preedit text appear as a modified line that differs from
                # both neighbours
                preedit_bg_colour = (diffcolours[flags - 1] * alpha).over(colour)

                if text is not None:
                    # draw char diffs
                    for starti, endi, start, w, colour in chardiff:
                        if has_preedit:
                            # make space for preedit text
                            if self.current_char <= starti:
                                start += preeditwidth
                            elif self.current_char < endi:
                                w += preeditwidth
                        cr.set_source_rgba(colour.red, colour.green, colour.blue, alpha)
                        cr.rectangle(x_start + pixels(start), y_start, pixels(w), h)
                        cr.fill()

                if has_preedit or (line is not None and line.is_modified):
                    # draw modified
                    colour = theResources.getColour('edited')
                    alpha = theResources.getFloat('edited_opacity')
                    preedit_bg_colour = (colour * alpha).over(preedit_bg_colour)
                    cr.set_source_rgba(colour.red, colour.green, colour.blue, alpha)
                    cr.paint()
                if self.mode == ALIGN_MODE:
                    # draw align
                    if self.align_pane == f and self.align_line == i:
                        colour = theResources.getColour('alignment')
                        alpha = theResources.getFloat('alignment_opacity')
                        cr.set_source_rgba(colour.red, colour.green, colour.blue, alpha)
                        cr.paint()
                elif self.mode == LINE_MODE:
                    # draw line selection
                    if self.current_pane == f:
                        start, end = self.selection_line, self.current_line
                        if end < start:
                            start, end = end, start
                        if i >= start and i <= end:
                            colour = theResources.getColour('line_selection')
                            alpha = theResources.getFloat('line_selection_opacity')
                            cr.set_source_rgba(colour.red, colour.green, colour.blue, alpha)
                            cr.paint()
                elif self.mode == CHAR_MODE:
                    # draw char selection
                    if self.current_pane == f and text is not None:
                        start, end = self.selection_line, self.current_line
                        start_char, end_char = self.selection_char, self.current_char
                        if end < start or (end == start and end_char < start_char):
                            start, start_char, end, end_char = end, end_char, start, start_char
                        if start <= i and end >= i:
                            if start < i:
                                start_char = 0
                            if end > i:
                                end_char = len(text)
                            if start_char < end_char:
                                if ss is None:
                                    ss = self.expand(text)
                                layout = self.create_pango_layout(''.join(ss[:start_char]))
                                layout.set_font_description(self.font)
                                x_temp = layout.get_size()[0]
                                layout = self.create_pango_layout(''.join(ss[start_char:end_char]))
                                layout.set_font_description(self.font)
                                w = layout.get_size()[0]
                                colour = theResources.getColour('character_selection')
                                alpha = theResources.getFloat('character_selection_opacity')
                                cr.set_source_rgba(colour.red, colour.green, colour.blue, alpha)
                                cr.rectangle(x_start + pixels(x_temp), y_start, pixels(w), h)
                                cr.fill()

                if self.prefs.getBool('display_show_right_margin'):
                    # draw margin
                    x_temp = line_number_width + pixels(self.prefs.getInt('display_right_margin') * self.digit_width)
                    if x_temp >= x and x_temp < maxx:
                        colour = theResources.getColour('margin')
                        cr.set_source_rgb(colour.red, colour.green, colour.blue)
                        cr.set_line_width(1)
                        cr.move_to(x_temp, y_start)
                        cr.rel_line_to(0, h)
                        cr.stroke()

                if text is None:
                    # draw hatching
                    colour = theResources.getColour('hatch')
                    cr.set_source_rgb(colour.red, colour.green, colour.blue)
                    cr.set_line_width(1)
                    h2 = 2 * h
                    temp = line_number_width
                    if temp < x:
                        temp += ((x - temp) // h) * h
                    h_half = 0.5 * h
                    phase = [ h_half, h_half, -h_half, -h_half ]
                    for j in range(4):
                        x_temp = temp
                        y_temp = y_start
                        for k in range(j):
                            y_temp += phase[k]
                        cr.move_to(x_temp, y_temp)
                        for k in range(j, 4):
                            cr.rel_line_to(h_half, phase[k])
                            x_temp += h_half
                        while x_temp < maxx:
                            cr.rel_line_to(h, h)
                            cr.rel_line_to(h, -h)
                            x_temp += h2
                        cr.stroke()
                else:
                    # continue populating the syntax highlighting cache until
                    # line 'i' is included
                    n = len(pane.syntax_cache)
                    while i >= n:
                        temp = self.getLineText(f, n)
                        if syntax is None:
                            initial_state, end_state = None, None
                            if temp is None:
                                blocks = None
                            else:
                                blocks = [ (0, len(temp), 'text') ]
                        else:
                            # apply the syntax highlighting rules to identify
                            # ranges of similarly coloured characters
                            if n == 0:
                                initial_state = syntax.initial_state
                            else:
                                initial_state = pane.syntax_cache[-1][1]
                            if temp is None:
                                end_state, blocks = initial_state, None
                            else:
                                end_state, blocks = syntax.parse(initial_state, temp)
                        pane.syntax_cache.append([initial_state, end_state, blocks, None])
                        n += 1

                    # use the cache the position, layout, and colour of each
                    # span of characters
                    blocks = pane.syntax_cache[i][3]
                    if blocks is None:
                        # populate the cache item if it didn't exist
                        if ss is None:
                            ss = self.expand(text)
                        x_temp = 0
                        blocks = []
                        for start, end, tag in pane.syntax_cache[i][2]:
                            layout = self.create_pango_layout(''.join(ss[start:end]))
                            layout.set_font_description(self.font)
                            colour = theResources.getColour(tag)
                            blocks.append((start, end, x_temp, layout, colour))
                            x_temp += layout.get_size()[0]
                        pane.syntax_cache[i][3] = blocks

                    # draw text
                    for starti, endi, start, layout, colour in blocks:
                        cr.set_source_rgb(colour.red, colour.green, colour.blue)
                        if has_preedit:
                            # make space for preedit text
                            if self.current_char <= starti:
                                start += preeditwidth
                            elif self.current_char < endi:
                                # divide text into 2 segments
                                ss = self.expand(text)
                                layout = self.create_pango_layout(''.join(ss[starti:self.current_char]))
                                layout.set_font_description(self.font)
                                cr.move_to(x_start + pixels(start), y_start)
                                cr.show_layout(layout)
                                start += layout.get_size()[0] + preeditwidth
                                layout = self.create_pango_layout(''.join(ss[self.current_char:endi]))
                                layout.set_font_description(self.font)
                        cr.move_to(x_start + pixels(start), y_start)
                        cr.show_layout(layout)

                if self.current_pane == f and self.current_line == i:
                    # draw the cursor and preedit text
                    if self.mode == CHAR_MODE:
                        x_pos = x_start + pixels(self._get_cursor_x_offset())
                        if has_preedit:
                            # we have preedit text
                            layout = self._preedit_layout()
                            w = pixels(layout.get_size()[0])

                            # clear the background
                            colour = preedit_bg_colour
                            cr.set_source_rgb(colour.red, colour.green, colour.blue)
                            cr.rectangle(x_pos, y_start, w, h)
                            cr.fill()
                            # draw the preedit text
                            colour = theResources.getColour('preedit')
                            cr.set_source_rgb(colour.red, colour.green, colour.blue)
                            cr.move_to(x_pos, y_start)
                            cr.show_layout(layout)
                            # advance to the preedit's cursor position
                            x_pos += pixels(self._preedit_layout(True).get_size()[0])
                        # draw the character editing cursor
                        colour = theResources.getColour('cursor')
                        cr.set_source_rgb(colour.red, colour.green, colour.blue)
                        cr.set_line_width(1)
                        cr.move_to(x_pos + 0.5, y_start)
                        cr.rel_line_to(0, h)
                        cr.stroke()
                    elif self.mode == LINE_MODE or self.mode == ALIGN_MODE:
                        # draw the line editing cursor
                        colour = theResources.getColour('cursor')
                        cr.set_source_rgb(colour.red, colour.green, colour.blue)
                        cr.set_line_width(1)
                        cr.move_to(maxx, y_start + 0.5)
                        cr.line_to(x_start + 0.5, y_start + 0.5)
                        cr.line_to(x_start + 0.5, y_start + h - 0.5)
                        cr.line_to(maxx, y_start + h - 0.5)
                        cr.stroke()
                cr.restore()
            # advance to the next exposed line
            i += 1
            y_start += h

        # draw the pixmap to window
        gc = pixmap.new_gc()
        widget.window.draw_drawable(gc, pixmap, 0, 0, offset_x, offset_y, width, height)

    # callback used when panes are scrolled horizontally
    def hadj_changed_cb(self, adj):
        self._cursor_position_changed(False)

    # callback used when panes are scrolled vertically
    def vadj_changed_cb(self, adj):
        self._cursor_position_changed(False)
        self.map.queue_draw()

    # callback to handle button presses on the overview map
    def map_button_press_cb(self, widget, event):
        vadj = self.vadj

        h = widget.get_allocation().height
        hmax = max(int(vadj.upper), h)

        # centre view about picked location
        y = event.y * hmax // h
        v = y - int(vadj.page_size / 2)
        v = max(v, int(vadj.lower))
        v = min(v, int(vadj.upper - vadj.page_size))
        vadj.set_value(v)

    # callback to handle mouse scrollwheel events
    def map_scroll_cb(self, widget, event):
        delta = 100
        if event.direction == gtk.gdk.SCROLL_UP:
            step_adjustment(self.vadj, -delta)
        elif event.direction == gtk.gdk.SCROLL_DOWN:
            step_adjustment(self.vadj, delta)

    # redraws the overview map when a portion is exposed
    def map_expose_cb(self, widget, event):
        n = len(self.panes)

        # compute map if it hasn't already been cached
        # the map is a list of (start, end, flags) tuples for each pane
        # flags & 1 indicates differences with the pane to the left
        # flags & 2 indicates differences with the pane to the right
        # flags & 4 indicates modified lines
        # flags & 8 indicates regular lines with text
        if self.map_cache is None:
            nlines = len(self.panes[0].lines)
            start = n * [ 0 ]
            flags = n * [ 0 ]
            self.map_cache = [ [] for f in range(n) ]
            # iterate over each row of lines
            for i in range(nlines):
                nextflag = 0
                # iterate over each pane
                for f in range(n):
                    flag = nextflag
                    nextflag = 0
                    s0 = self.getCompareString(f, i)
                    # compare with neighbour to the right
                    if f + 1 < n:
                        if s0 != self.getCompareString(f + 1, i):
                            flag |= 2
                            nextflag |= 1
                    line = self.getLine(f, i)
                    if line is not None and line.is_modified:
                        # modified line
                        flag = 4
                    elif line is None or line.getText() is None:
                        # empty line
                        flag = 0
                    elif flag == 0:
                        # regular line
                        flag = 8
                    if flags[f] != flag:
                        if flags[f] != 0:
                            self.map_cache[f].append([start[f], i, flags[f]])
                        start[f] = i
                        flags[f] = flag
            # finish any incomplete ranges
            for f in range(n):
                if flags[f] != 0:
                    self.map_cache[f].append([start[f], nlines, flags[f]])

        # draw to a pixmap to avoid screen flicker
        x, y, width, height = event.area
        pixmap = gtk.gdk.Pixmap(widget.window, width, height)
        cr = pixmap.cairo_create()
        cr.translate(-x, -y)

        # clear
        colour = theResources.getColour('map_background')
        cr.set_source_rgb(colour.red, colour.green, colour.blue)
        cr.paint()
        bg_colour = theResources.getColour('text_background')
        edited_colour = theResources.getColour('edited')

        # get scroll position and total size
        r = widget.get_allocation()
        w, h = r.width, r.height
        vadj = self.vadj
        hmax = max(vadj.upper, h)

        # draw diff blocks
        wn = w / n
        pad = 1
        for f in range(n):
            diffcolours = [ theResources.getDifferenceColour(f), theResources.getDifferenceColour(f + 1) ]
            diffcolours.append((diffcolours[0] + diffcolours[1]) * 0.5)
            wx = f * wn
            # draw in two passes, more important stuff in the second pass
            # this ensures less important stuff does not obscure more important
            # data
            for p in range(2):
                for start, end, flag in self.map_cache[f]:
                    if p == 0 and flag == 8:
                        colour = bg_colour
                    elif p == 1 and flag & 7:
                        if flag & 4:
                            colour = edited_colour
                        else:
                            colour = diffcolours[(flag & 3) - 1]
                    else:
                        continue

                    # ensure the line is visible in the map
                    ymin = h * self.font_height * start // hmax
                    if ymin >= y + height:
                        break
                    yh = max(h * self.font_height * end // hmax - ymin, 1)
                    if ymin + yh <= y:
                        continue

                    cr.set_source_rgb(colour.red, colour.green, colour.blue)
                    cr.rectangle(wx + pad, ymin, wn - 2 * pad, yh)
                    cr.fill()

        # draw cursor
        vmin = int(vadj.get_value())
        vmax = vmin + vadj.page_size
        ymin = h * vmin // hmax
        if ymin < y + height:
            yh = h * vmax // hmax - ymin
            if yh > 1:
                yh -= 1
            if ymin + yh > y:
                colour = theResources.getColour('line_selection')
                alpha = theResources.getFloat('line_selection_opacity')
                cr.set_source_rgba(colour.red, colour.green, colour.blue, alpha)
                cr.rectangle(0.5, ymin + 0.5, w - 1, yh - 1)
                cr.fill()

                colour = theResources.getColour('cursor')
                cr.set_source_rgb(colour.red, colour.green, colour.blue)
                cr.set_line_width(1)
                cr.rectangle(0.5, ymin + 0.5, w - 1, yh - 1)
                cr.stroke()

        # draw the pixmap to the window
        gc = pixmap.new_gc()
        widget.window.draw_drawable(gc, pixmap, 0, 0, x, y, width, height)

    # returns the maximum valid offset for a cursor position
    # cursors cannot be moved to the right of line ending characters
    def getMaxCharPosition(self, i):
        return len_minus_line_ending(self.getLineText(self.current_pane, i))

    def _line_mode_enter_align_mode(self):
        if self.mode == CHAR_MODE:
            self._im_focus_out()
            self.im_context.reset()
            self._im_set_preedit(None)
        self.mode = ALIGN_MODE
        self.selection_line = self.current_line
        self.align_pane = self.current_pane
        self.align_line = self.current_line
        self.emit('mode_changed')
        self.dareas[self.align_pane].queue_draw()

    def _first_line(self):
        self.setCurrentLine(self.current_pane, 0)

    def _extend_first_line(self):
        self.setCurrentLine(self.current_pane, 0, self.selection_line)

    def _last_line(self):
        f = self.current_pane
        self.setCurrentLine(f, len(self.panes[f].lines))

    def _extend_last_line(self):
        f = self.current_pane
        self.setCurrentLine(f, len(self.panes[f].lines), self.selection_line)

    def _line_mode_up(self, selection=None):
        self.setCurrentLine(self.current_pane, self.current_line - 1, selection)

    def _line_mode_extend_up(self):
        self._line_mode_up(self.selection_line)

    def _line_mode_down(self, selection=None):
        self.setCurrentLine(self.current_pane, self.current_line + 1, selection)

    # 'extend_down' keybinding action
    def _line_mode_extend_down(self):
        self._line_mode_down(self.selection_line)

    def _line_mode_left(self, selection=None):
        self.setCurrentLine(self.current_pane - 1, self.current_line, selection)

    # 'extend_left' keybinding action
    def _line_mode_extend_left(self):
        self._line_mode_left(self.selection_line)

    def _line_mode_right(self, selection=None):
        self.setCurrentLine(self.current_pane + 1, self.current_line, selection)

    # 'extend_right' keybinding action
    def _line_mode_extend_right(self):
        self._line_mode_right(self.selection_line)

    def _line_mode_page_up(self, selection=None):
        delta = int(self.vadj.page_size // self.font_height)
        self.setCurrentLine(self.current_pane, self.current_line - delta, selection)

    # 'extend_page_up' keybinding action
    def _line_mode_extend_page_up(self):
        self._line_mode_page_up(self.selection_line)

    def _line_mode_page_down(self, selection=None):
        delta = int(self.vadj.page_size // self.font_height)
        self.setCurrentLine(self.current_pane, self.current_line + delta, selection)

    # 'extend_page_down' keybinding action
    def _line_mode_extend_page_down(self):
        self._line_mode_page_down(self.selection_line)

    # 'delete_text' keybinding action
    def _delete_text(self):
        self.replaceText('')

    def _align_mode_enter_line_mode(self):
        self.selection_line = self.current_line
        self.setLineMode()

    def _align_text(self):
        f1 = self.align_pane
        line1 = self.align_line
        line2 = self.current_line
        self.selection_line = line2
        self.setLineMode()
        if self.current_pane == f1 + 1:
            self.align(f1, line1, line2)
        elif self.current_pane + 1 == f1:
            self.align(self.current_pane, line2, line1)

    # give the input method focus
    def _im_focus_in(self):
        if self.has_focus:
            self.im_context.focus_in()

    # remove input method focus
    def _im_focus_out(self):
        if self.has_focus:
            self.im_context.focus_out()

    # input method callback for committed text
    def im_commit_cb(self, im, s):
        if self.mode == CHAR_MODE:
            self.openUndoBlock()
            self.replaceText(unicode(s, 'utf_8'))
            self.closeUndoBlock()

    # update the cached preedit text
    def _im_set_preedit(self, p):
        self.im_preedit = p
        if self.mode == CHAR_MODE:
            f, i = self.current_pane, self.current_line
            self._queue_draw_lines(f, i)
            if f > 0:
                self._queue_draw_lines(f - 1, i)
            if f + 1 < len(self.panes):
                self._queue_draw_lines(f + 1, i)
        self.updateSize(False)

    # queue a redraw for location of preedit text
    def im_preedit_changed_cb(self, im):
        if self.mode == CHAR_MODE:
            s, a, c = self.im_context.get_preedit_string()
            if len(s) > 0:
                # we have preedit text, draw that instead
                s = unicode(s, 'utf_8')
                p = (s, a, c)
            else:
                p = None
            self._im_set_preedit(p)
            self._ensure_cursor_is_visible()

    # callback to respond to retrieve_surrounding signals from input methods
    def im_retrieve_surrounding_cb(self, im):
        if self.mode == CHAR_MODE:
            # notify input method of text surrounding the cursor
            s = nullToEmpty(self.getLineText(self.current_pane, self.current_line))
            s, n = codecs.encode(s, 'utf_8'), len(codecs.encode(s[:self.current_char], 'utf_8'))
            im.set_surrounding(s, len(s), n)

    # callback for 'focus_in_event'
    def focus_in_cb(self, widget, event):
        self.has_focus = True
        if self.mode == CHAR_MODE:
            # notify the input method of the focus change
            self._im_focus_in()

    # callback for 'focus_out_event'
    def focus_out_cb(self, widget, event):
        if self.mode == CHAR_MODE:
            # notify the input method of the focus change
            self._im_focus_out()
        self.has_focus = False

    # callback for keyboard events
    # only keypresses that are not handled by menu item accelerators reach here
    def key_press_cb(self, widget, event):
        if self.mode == CHAR_MODE:
            # update input method
            if self.im_context.filter_keypress(event):
                return True
        retval = False
        # determine the modified keys used
        mask = event.state & (gtk.gdk.SHIFT_MASK | gtk.gdk.CONTROL_MASK)
        if event.state & gtk.gdk.LOCK_MASK:
            mask ^= gtk.gdk.SHIFT_MASK
        self.openUndoBlock()
        if self.mode == LINE_MODE:
            # check if the keyval matches a line mode action
            action = theResources.getActionForKey('line_mode', event.keyval, mask)
            if action in self._line_mode_actions:
                self._line_mode_actions[action]()
                retval = True
        elif self.mode == CHAR_MODE:
            f = self.current_pane
            if event.state & gtk.gdk.SHIFT_MASK:
                si, sj = self.selection_line, self.selection_char
            else:
                si, sj = None, None
            is_ctrl = event.state & gtk.gdk.CONTROL_MASK
            retval = True
            # check if the keyval matches a character mode action
            action = theResources.getActionForKey('character_mode', event.keyval, mask)
            if action in self._character_mode_actions:
                self._character_mode_actions[action]()
            # allow CTRL-Tab for widget navigation
            elif event.keyval == gtk.keysyms.Tab and event.state & gtk.gdk.CONTROL_MASK:
                retval = False
            # up/down cursor navigation
            elif event.keyval in [ gtk.keysyms.Up, gtk.keysyms.Down, gtk.keysyms.Page_Up, gtk.keysyms.Page_Down ]:
                i = self.current_line
                # move back to the remembered cursor column if possible
                col = self.cursor_column
                if col < 0:
                    # find the current cursor column
                    s = nullToEmpty(self.getLineText(f, i))[:self.current_char]
                    col = self.stringWidth(s)
                if event.keyval in [ gtk.keysyms.Up, gtk.keysyms.Down ]:
                    delta = 1
                else:
                    delta = int(self.vadj.page_size // self.font_height)
                if event.keyval in [ gtk.keysyms.Up, gtk.keysyms.Page_Up ]:
                    delta = -delta
                i += delta
                j = 0
                nlines = len(self.panes[f].lines)
                if i < 0:
                    i = 0
                elif i > nlines:
                    i = nlines
                else:
                    # move the cursor to column 'col' if possible
                    s = self.getLineText(f, i)
                    if s is not None:
                        s = strip_eol(s)
                        idx = 0
                        for c in s:
                            w = self.characterWidth(idx, c)
                            if idx + w > col:
                                break
                            idx += w
                            j += 1
                self.setCurrentChar(i, j, si, sj)
                self.cursor_column = col
            # home key
            elif event.keyval == gtk.keysyms.Home:
                if is_ctrl:
                    i = 0
                else:
                    i = self.current_line
                self.setCurrentChar(i, 0, si, sj)
            # end key
            elif event.keyval == gtk.keysyms.End:
                if is_ctrl:
                    i = len(self.panes[f].lines)
                    j = 0
                else:
                    i = self.current_line
                    j = self.getMaxCharPosition(i)
                self.setCurrentChar(i, j, si, sj)
            # cursor left and cursor right navigation
            elif event.keyval == gtk.keysyms.Left or event.keyval == gtk.keysyms.Right:
                i = self.current_line
                j = self.current_char
                while True:
                    if event.keyval == gtk.keysyms.Left:
                        if j > 0:
                            j -= 1
                        elif i > 0:
                            i -= 1
                            j = self.getMaxCharPosition(i)
                        else:
                            break
                    else:
                        if j < self.getMaxCharPosition(i):
                            j += 1
                        elif i < len(self.panes[f].lines):
                            i += 1
                            j = 0
                        else:
                            break
                    if event.state & gtk.gdk.CONTROL_MASK == 0:
                        break
                    # break if we are at the beginning of a word
                    text = self.getLineText(f, i)
                    if text is not None and j < len(text):
                        c = getCharacterClass(text[j])
                        if c != WHITESPACE_CLASS and (j < 1 or j - 1 >= len(text) or getCharacterClass(text[j - 1]) != c):
                            break
                self.setCurrentChar(i, j, si, sj)
            # backspace
            elif event.keyval == gtk.keysyms.BackSpace:
                s = ''
                i = self.current_line
                j = self.current_char
                if self.selection_line == i and self.selection_char == j:
                    if j > 0:
                        # delete back to the last soft-tab location if there
                        # are only spaces and tabs from the beginning of the
                        # line to the current cursor position
                        text = self.getLineText(f, i)[:j]
                        for c in text:
                            if c not in ' \t':
                                j -= 1
                                break
                        else:
                            w = self.stringWidth(text)
                            width = self.prefs.getInt('editor_soft_tab_width')
                            w = (w - 1) // width * width
                            if self.prefs.getBool('editor_expand_tabs'):
                                s = u' ' * w
                            else:
                                width = self.prefs.getInt('display_tab_width')
                                s = u'\t' * (w // width) + u' ' * (w % width)
                            j = 0
                    else:
                        # delete back to an end of line character from the
                        # previous line
                        while i > 0:
                            i -= 1
                            text = self.getLineText(f, i)
                            if text is not None:
                                j = self.getMaxCharPosition(i)
                                break
                    self.current_line = i
                    self.current_char = j
                self.replaceText(s)
            # delete key
            elif event.keyval == gtk.keysyms.Delete:
                i = self.current_line
                j = self.current_char
                if self.selection_line == i and self.selection_char == j:
                    # advance the selection to the next character so we can
                    # delete it
                    text = self.getLineText(f, i)
                    while text is None and i < len(self.panes[f].lines):
                        i += 1
                        j = 0
                        text = self.getLineText(f, i)
                    if text is not None:
                        if j < self.getMaxCharPosition(i):
                            j += 1
                        else:
                            i += 1
                            j = 0
                    self.current_line = i
                    self.current_char = j
                self.replaceText('')
            # return key, add the platform specific end of line characters
            elif event.keyval in [ gtk.keysyms.Return, gtk.keysyms.KP_Enter ]:
                s = unicode(os.linesep)
                if self.prefs.getBool('editor_auto_indent'):
                    start_i, start_j = self.selection_line, self.selection_char
                    end_i, end_j = self.current_line, self.current_char
                    if end_i < start_i or (end_i == start_i and end_j < start_j):
                        start_i, start_j = end_i, end_j
                    if start_j > 0:
                        j, text = 0, self.getLineText(f, start_i)
                        while j < start_j and text[j] in ' \t':
                            j += 1
                        w = self.stringWidth(text[:j])
                        if self.prefs.getBool('editor_expand_tabs'):
                            # convert to spaces
                            s += u' ' * w
                        else:
                            tab_width = self.prefs.getInt('display_tab_width')
                            # replace with tab characters where possible
                            s += u'\t' * (w // tab_width)
                            s += u' ' * (w % tab_width)
                self.replaceText(s)
            # insert key
            elif event.keyval in [ gtk.keysyms.Tab, gtk.keysyms.ISO_Left_Tab ]:
                start_i, start_j = self.selection_line, self.selection_char
                end_i, end_j = self.current_line, self.current_char
                if start_i != end_i or start_j != end_j or event.keyval == gtk.keysyms.ISO_Left_Tab:
                    # find range of lines to operate upon
                    start, end, offset = start_i, end_i, 1
                    if end < start:
                        start, end = end, start
                    if event.keyval == gtk.keysyms.ISO_Left_Tab:
                        offset = -1
                    self.recordEditMode()
                    for i in range(start, end + 1):
                        text = self.getLineText(f, i)
                        if text is not None and len_minus_line_ending(text) > 0:
                            # count spacing before the first non-whitespace character
                            j, w = 0, 0
                            while j < len(text) and text[j] in ' \t':
                                w += self.characterWidth(w, text[j])
                                j += 1
                            # adjust by a multiple of the soft tab width
                            ws = max(0, w + offset * self.prefs.getInt('editor_soft_tab_width'))
                            if ws != w:
                                if self.prefs.getBool('editor_expand_tabs'):
                                    s = u' ' * ws
                                else:
                                    tab_width = self.prefs.getInt('display_tab_width')
                                    s = u'\t' * (ws // tab_width) + u' ' * (ws % tab_width)
                                if i == start_i:
                                    start_j = len(s) + max(0, start_j - j)
                                if i == end_i:
                                    end_j = len(s) + max(0, end_j - j)
                                self.updateText(f, i, s + text[j:])
                    self.setCurrentChar(end_i, end_j, start_i, start_j)
                    self.recordEditMode()
                else:
                    # insert soft-tabs if there are only spaces and tabs from the
                    # beginning of the line to the cursor location
                    if end_i < start_i or (end_i == start_i and end_j < start_j):
                        start_i, start_j, end_i, end_j = end_i, end_j, start_i, start_j
                    temp = start_j
                    if temp > 0:
                        text = self.getLineText(f, start_i)[:start_j]
                        w = self.stringWidth(text)
                        while temp > 0 and text[temp - 1] in ' \t':
                            temp -= 1
                    else:
                        w = 0
                    tab_width = self.prefs.getInt('display_tab_width')
                    if temp > 0:
                        # insert a regular tab
                        ws = tab_width - w % tab_width
                    else:
                        # insert a soft tab
                        self.selection_line = start_i
                        self.selection_char = 0
                        self.current_line = end_i
                        self.current_char = end_j
                        width = self.prefs.getInt('editor_soft_tab_width')
                        ws = w + width - w % width
                        w = 0
                    if self.prefs.getBool('editor_expand_tabs'):
                        # convert to spaces
                        s = u' ' * ws
                    else:
                        # replace with tab characters where possible
                        s = u'\t' * ((w + ws) // tab_width - w // tab_width)
                        s += u' ' * ((w + ws) % tab_width)
                    self.replaceText(s)
            # handle all other printable characters
            elif len(event.string) > 0:
                self.replaceText(event.string)
        elif self.mode == ALIGN_MODE:
            # check if the keyval matches an align mode action
            action = theResources.getActionForKey('align_mode', event.keyval, mask)
            if action in self._align_mode_actions:
                self._align_mode_actions[action]()
                retval = True
        self.closeUndoBlock()
        return retval

    # 'copy' action
    def copy(self):
        if self.mode == LINE_MODE or self.mode == CHAR_MODE:
            self.__set_clipboard_text(gtk.gdk.SELECTION_CLIPBOARD, self.getSelectedText())

    # 'cut' action
    def cut(self):
        if self.mode == LINE_MODE or self.mode == CHAR_MODE:
            self.copy()
            self.replaceText('')

    # callback used when receiving clipboard text
    def receive_clipboard_text_cb(self, clipboard, text, data):
        if self.mode == LINE_MODE or self.mode == CHAR_MODE:
            # there is no guarantee this will be called before finishing
            # gtk.clipboard_get so we may need to create our own undo block
            needs_block = (self.undoblock is None)
            if needs_block:
                self.openUndoBlock()
            self.replaceText(unicode(nullToEmpty(text), 'utf_8'))
            if needs_block:
                self.closeUndoBlock()

    # 'paste' action
    def paste(self):
         gtk.clipboard_get(gtk.gdk.SELECTION_CLIPBOARD).request_text(self.receive_clipboard_text_cb)

    # 'clear_edits' action
    def clear_edits(self):
        self.setLineMode()
        self.recordEditMode()
        f = self.current_pane
        start, end = self.selection_line, self.current_line
        if end < start:
            start, end = end, start
        end = min(end + 1, len(self.panes[0].lines))
        for i in range(start, end):
            line = self.getLine(f, i)
            if line is not None and line.is_modified:
                # remove the edits to the line
                self.updateText(f, i, None, False)
                if line.text is None:
                    # remove the line so it doesn't persist as a spacer
                    self.instanceLine(f, i, True)
        self.recordEditMode()

    # 'dismiss_all_edits' action
    def dismiss_all_edits(self):
        if self.mode == LINE_MODE or self.mode == CHAR_MODE:
            self.bakeEdits(self.current_pane)

    # callback for find menu item
    def find(self, pattern, match_case, backwards, from_start):
        self.setCharMode()
        # determine where to start searching from
        f = self.current_pane
        nlines = len(self.panes[f].lines)
        i, j = self.current_line, self.current_char
        si, sj = self.selection_line, self.selection_char
        if backwards:
            if from_start:
                i, j = nlines, 0
            elif si < i or (i == si and sj < j):
                i, j = si, sj
        else:
            if from_start:
                i, j = 0, 0
            elif i < si or (i == si and j < sj):
                i, j = si, sj

        if not match_case:
            pattern = pattern.upper()

        # iterate over all valid lines
        while i < nlines + 1:
            text = self.getLineText(f, i)
            if text is not None:
                if not match_case:
                    text = text.upper()
                # search for pattern
                if backwards:
                    idx = text.rfind(pattern, 0, j)
                else:
                    idx = text.find(pattern, j)
                if idx >= 0:
                    # we found a match
                    end = idx + len(pattern)
                    if backwards:
                        idx, end = end, idx
                    self.setCurrentChar(i, end, i, idx)
                    return True
            # advance
            if backwards:
                if i == 0:
                    break
                i -= 1
                text = self.getLineText(f, i)
                if text is None:
                    j = 0
                else:
                    j = len(text)
            else:
                i += 1
                j = 0
        # we have reached the end without finding a match
        return False

    # move cursor to a given line
    def go_to_line(self, i):
        f, idx = self.current_pane, 0
        if i > 0:
            # search for a line matching that number
            # we want to leave the cursor at the end of the file
            # if 'i' is greater than the last numbered line
            lines = self.panes[f].lines
            while idx < len(lines):
                line = lines[idx]
                if line is not None and line.line_number == i:
                    break
                idx += 1
        # select the line and make sure it is visible
        self.setLineMode()
        self.centre_view_about_y((idx + 0.5) * self.font_height)
        self.setCurrentLine(f, idx)

    # recompute viewport size and redraw as the display preferences may have
    # changed
    def prefsUpdated(self):
        # clear cache as tab width may have changed
        self.string_width_cache = {}
        self.setFont(pango.FontDescription(self.prefs.getString('display_font')))
        # update preedit text
        self._cursor_position_changed(True)

        for pane in self.panes:
            del pane.diff_cache[:]
        # tab width may have changed
        self.emit('cursor_changed')
        for darea in self.dareas:
            darea.queue_draw()
        self.map_cache = None
        self.map.queue_draw()

    # 'realign_all' action
    def realign_all(self):
        self.setLineMode()
        f = self.current_pane
        self.recordEditMode()
        lines = []
        blocks = []
        for pane in self.panes:
            # create a new list of lines with no spacers
            newlines = [ [ line for line in pane.lines if line is not None ] ]
            newblocks = createBlock(len(newlines[0]))
            if len(lines) > 0:
                # match with neighbour to the left
                self.alignBlocks(blocks, lines, newblocks, newlines)
                blocks = mergeBlocks(blocks, newblocks)
            else:
                blocks = newblocks
            lines.extend(newlines)
        self.updateAlignment(0, len(self.panes[f].lines), lines)
        self.updateBlocks(blocks)
        self.setCurrentLine(f, min(self.current_line, len(self.panes[f].lines)))
        self.recordEditMode()

    # callback for the align with selection menu item
    def align_with_selection_cb(self, widget, data):
        self.setLineMode()
        self.openUndoBlock()
        self.recordEditMode()
        # get the line and pane where the user right clicked
        f, line1 = data
        f2 = self.current_pane
        line2 = self.current_line
        if f2 < f:
            f = f2
            line1, line2 = line2, line1
        self.align(f, line1, line2)
        self.recordEditMode()
        self.closeUndoBlock()

    # 'isolate' action
    def isolate(self):
        self.setLineMode()
        self.recordEditMode()
        f = self.current_pane
        start, end = self.selection_line, self.current_line
        if end < start:
            start, end = end, start
        end += 1
        nlines = len(self.panes[f].lines)
        end = min(end, nlines)
        n = end - start
        if n > 0:
            lines = [ pane.lines[start:end] for pane in self.panes ]
            space = [ n * [ None ] for pane in self.panes ]
            lines[f], space[f] = space[f], lines[f]

            pre, post = cutBlocks(end, self.blocks)
            pre, middle = cutBlocks(start, pre)

            # remove nulls
            b = createBlock(n)
            removeNullLines(b, space)
            end = start + sum(b)
            if end > start:
                end -= 1
            removeNullLines(middle, lines)

            for s, line in zip(space, lines):
                s.extend(line)

            # update lines and blocks
            self.updateAlignment(start, n, space)
            pre.extend(b)
            pre.extend(middle)
            pre.extend(post)
            self.updateBlocks(pre)
            self.removeSpacerLines(end, sum(middle))
            end -= self.removeSpacerLines(start, sum(b))
            self.setCurrentLine(f, end, start)
        self.recordEditMode()

    # returns True if line 'i' in pane 'f' has an edit or is different from its
    # neighbour
    def hasEditsOrDifference(self, f, i):
        line = self.getLine(f, i)
        if line is not None and line.is_modified:
            return True
        text = self.getCompareString(f, i)
        return (f > 0 and self.getCompareString(f - 1, i) != text) or (f + 1 < len(self.panes) and text != self.getCompareString(f + 1, i))

    def hasDifferences(self):
        n = len(self.panes)
        nlines = len(self.panes[0].lines)
        for i in range(nlines):
            text = self.getCompareString(0, i)
            for f in range(1, n):
                if self.getCompareString(f, i) != text:
                    return True
        return False

    # scroll the viewport so pixels at position 'y' are centred
    def centre_view_about_y(self, y):
        vadj = self.vadj
        y = min(max(0, y - vadj.page_size / 2), vadj.upper - vadj.page_size)
        vadj.set_value(y)

    # move the cursor from line 'i' to the next difference in direction 'delta'
    def go_to_difference(self, i, delta):
        f = self.current_pane
        nlines = len(self.panes[f].lines)
        # back up to beginning of difference
        if i >= 0 and i <= nlines:
            while self.hasEditsOrDifference(f, i):
                i2 = i - delta
                if i2 < 0 or i2 > nlines:
                    break
                i = i2
        # step over non-difference
        while i >= 0 and i <= nlines and not self.hasEditsOrDifference(f, i):
            i += delta
        # find extent of difference
        if i >= 0 and i <= nlines:
            start = i
            while i >= 0 and i <= nlines and self.hasEditsOrDifference(f, i):
                i += delta
            i -= delta
            if i < start:
                start, i = i, start
            # centre the view on the selection
            self.centre_view_about_y((start + i) * self.font_height / 2)
            self.setCurrentLine(f, start, i)

    # 'first_difference' action
    def first_difference(self):
        self.setLineMode()
        self.go_to_difference(0, 1)

    # 'previous_difference' action
    def previous_difference(self):
        self.setLineMode()
        i = min(self.current_line, self.selection_line) - 1
        self.go_to_difference(i, -1)

    # 'next_difference' action
    def next_difference(self):
        self.setLineMode()
        i = max(self.current_line, self.selection_line) + 1
        self.go_to_difference(i, 1)

    # 'last_difference' action
    def last_difference(self):
        self.setLineMode()
        i = len(self.panes[self.current_pane].lines)
        self.go_to_difference(i, -1)

    # Undo for changes to the pane ordering
    class SwapPanesUndo:
        def __init__(self, f_dst, f_src):
            self.data = (f_dst, f_src)

        def undo(self, viewer):
            f_dst, f_src = self.data
            viewer.swapPanes(f_src, f_dst)

        def redo(self, viewer):
            f_dst, f_src = self.data
            viewer.swapPanes(f_dst, f_src)

    # swap the contents of two panes
    def swapPanes(self, f_dst, f_src):
        if self.undoblock is not None:
            self.addUndo(FileDiffViewer.SwapPanesUndo(f_dst, f_src))
        self.current_pane = f_dst
        f0 = self.panes[f_dst]
        f1 = self.panes[f_src]
        self.panes[f_dst], self.panes[f_src] = f1, f0
        npanes = len(self.panes)
        for f_idx in f_dst, f_src:
            for f in range(f_idx - 1, f_idx + 2):
                if f >= 0 and f < npanes:
                    # clear the diff cache and redraw as the pane has a new
                    # neighour
                    del self.panes[f].diff_cache[:]
                    self.dareas[f].queue_draw()
        # queue redraw
        self.map_cache = None
        self.map.queue_draw()
        self.emit('swapped_panes', f_dst, f_src)

    # swap the contents of two panes
    def swap_panes(self, f_dst, f_src):
        if f_dst >= 0 and f_dst < len(self.panes):
            if self.mode == ALIGN_MODE:
                self.setLineMode()
            self.recordEditMode()
            self.swapPanes(f_dst, f_src)
            self.recordEditMode()

    # callback for swap panes menu item
    def swap_panes_cb(self, widget, data):
        self.openUndoBlock()
        self.swap_panes(data, self.current_pane)
        self.closeUndoBlock()

    # 'shift_pane_left' action
    def shift_pane_left(self):
        f = self.current_pane
        self.swap_panes(f - 1, f)

    # 'shift_pane_right' action
    def shift_pane_right(self):
        f = self.current_pane
        self.swap_panes(f + 1, f)

    # 'convert_to_upper_case' action
    def _convert_case(self, to_upper):
        # find range of characters to operate upon
        if self.mode == CHAR_MODE:
            start, end = self.current_line, self.selection_line
            j0, j1 = self.current_char, self.selection_char
            if end < start or (start == end and j1 < j0):
                start, j0, end, j1 = end, j1, start, j0
        else:
            self.setLineMode()
            start, end = self.current_line, self.selection_line
            if end < start:
                start, end = end, start
            end += 1
            j0, j1 = 0, 0
        self.recordEditMode()
        f = self.current_pane
        for i in range(start, end + 1):
            text = self.getLineText(f, i)
            if text is not None:
                s = text
                # skip characters after the selection
                if i == end:
                    s, post = s[:j1], s[j1:]
                else:
                    post = ''
                # skip characters before the selection
                if i == start:
                    pre, s = s[:j0], s[j0:]
                else:
                    pre = ''
                # change the case
                if to_upper:
                    s = s.upper()
                else:
                    s = s.lower()
                s = ''.join([pre, s, post])
                # only update the line if it changed
                if s != text:
                    self.updateText(f, i, s)

    # 'convert_to_upper_case' action
    def convert_to_upper_case(self):
        self._convert_case(True)

    # 'convert_to_lower_case' action
    def convert_to_lower_case(self):
        self._convert_case(False)

    # sort lines
    def _sort_lines(self, descending):
        if self.mode != CHAR_MODE:
            self.setLineMode()
        self.recordEditMode()
        f = self.current_pane
        # find cursor range
        start, end = self.selection_line, self.current_line
        if end < start:
            start, end = end, start
        # get set of lines
        ss = [ self.getLineText(f, i) for i in range(start, end + 1) ]
        # create sorted list, removing any nulls
        temp = [ s for s in ss if s is not None ]
        temp.sort()
        if descending:
            temp.reverse()
        # add back in the nulls
        temp.extend((len(ss) - len(temp)) * [ None ])
        for i, s in enumerate(temp):
            # update line if it changed
            if ss[i] != s:
                self.updateText(f, start + i, s)
        if self.mode == CHAR_MODE:
            # ensure the cursor position is valid
            self.setCurrentChar(self.current_line, 0, self.selection_line, 0)
        self.recordEditMode()

    # 'sort_lines_in_ascending_order' action
    def sort_lines_in_ascending_order(self):
        self._sort_lines(False)

    # 'sort_lines_in_descending_order' action
    def sort_lines_in_descending_order(self):
        self._sort_lines(True)

    # 'remove_trailing_white_space' action
    def remove_trailing_white_space(self):
        if self.mode != CHAR_MODE:
            self.setLineMode()
        self.recordEditMode()
        f = self.current_pane
        # find cursor range
        start, end = self.selection_line, self.current_line
        if end < start:
            start, end = end, start
        # get set of lines
        for i in range(start, end + 1):
            text = self.getLineText(f, i)
            if text is not None:
                # locate trailing whitespace
                old_n = n = len_minus_line_ending(text)
                while n > 0 and text[n - 1] in whitespace:
                    n -= 1
                # update line if it changed
                if n < old_n:
                    self.updateText(f, i, text[:n] + text[old_n:])
        if self.mode == CHAR_MODE:
            # ensure the cursor position is valid
            self.setCurrentChar(self.current_line, 0, self.selection_line, 0)
        self.recordEditMode()

    # 'convert_tabs_to_spaces' action
    def convert_tabs_to_spaces(self):
        # find range of characters to operate upon
        if self.mode == CHAR_MODE:
            start, end = self.current_line, self.selection_line
            j0, j1 = self.current_char, self.selection_char
            if end < start or (start == end and j1 < j0):
                start, j0, end, j1 = end, j1, start, j0
        else:
            self.setLineMode()
            start, end = self.current_line, self.selection_line
            if end < start:
                start, end = end, start
            end += 1
            j0, j1 = 0, 0
        self.recordEditMode()
        f = self.current_pane
        for i in range(start, end + 1):
            text = self.getLineText(f, i)
            if text is not None:
                # expand tabs
                ss, col = [], 0
                for c in text:
                    w = self.characterWidth(col, c)
                    # replace tab with spaces
                    if c == '\t':
                        c = w * u' '
                    ss.append(c)
                    col += w
                # determine the range of interest
                if i == start:
                    k0 = j0
                else:
                    k0 = 0
                if i == end:
                    k1 = j1
                else:
                    k1 = len(ss)
                # compute leading and converted text
                s = text[:k0] + ''.join(ss[k0:k1])
                if i == end:
                    # update the end cursor location
                    j1 = len(s)
                # append the trailing text
                s += text[k1:]
                # update line only if it changed
                if text != s:
                    self.updateText(f, i, s)
        if self.mode == CHAR_MODE:
            # ensure the cursor position is valid
            self.setCurrentChar(end, j1, start, j0)
        self.recordEditMode()

    # 'convert_leading_spaces_to_tabs' action
    def convert_leading_spaces_to_tabs(self):
        if self.mode != CHAR_MODE:
            self.setLineMode()
        self.recordEditMode()
        f = self.current_pane
        tab_width = self.prefs.getInt('display_tab_width')
        # find cursor range
        start, end = self.selection_line, self.current_line
        if end < start:
            start, end = end, start
        for i in range(start, end + 1):
            text = self.getLineText(f, i)
            if text is not None:
                # find leading white space
                j, col = 0, 0
                while j < len(text) and text[j] in ' \t':
                    col += self.characterWidth(col, text[j])
                    j += 1
                if col >= tab_width:
                    # convert to tabs
                    s = ''.join([ u'\t' * (col // tab_width), u' ' * (col % tab_width), text[j:] ])
                    # update line only if it changed
                    if text != s:
                        self.updateText(f, i, s)
        if self.mode == CHAR_MODE:
            # ensure the cursor position is valid
            self.setCurrentChar(self.current_line, 0, self.selection_line, 0)
        self.recordEditMode()

    # adjust indenting of the selected lines by 'offset' soft tabs
    def _adjust_indenting(self, offset):
        if self.mode != CHAR_MODE:
            self.setLineMode()
        # find range of lines to operate upon
        f = self.current_pane
        start, end = self.selection_line, self.current_line
        if end < start:
            start, end = end, start

        self.recordEditMode()
        for i in range(start, end + 1):
            text = self.getLineText(f, i)
            if text is not None and len_minus_line_ending(text) > 0:
                # count spacing before the first non-whitespace character
                j, w = 0, 0
                while j < len(text) and text[j] in ' \t':
                    w += self.characterWidth(w, text[j])
                    j += 1
                # adjust by a multiple of the soft tab width
                ws = max(0, w + offset * self.prefs.getInt('editor_soft_tab_width'))
                if ws != w:
                    if self.prefs.getBool('editor_expand_tabs'):
                        s = u' ' * ws
                    else:
                        tab_width = self.prefs.getInt('display_tab_width')
                        s = u'\t' * (ws // tab_width) + u' ' * (ws % tab_width)
                    self.updateText(f, i, s + text[j:])
        if self.mode == CHAR_MODE:
            # ensure the cursor position is valid
            self.setCurrentChar(self.current_line, 0, self.selection_line, 0)
        self.recordEditMode()

    # 'increase_indenting' action
    def increase_indenting(self):
        self._adjust_indenting(1)

    # 'decrease_indenting' action
    def decrease_indenting(self):
        self._adjust_indenting(-1)

    def convert_format(self, format):
        self.setLineMode()
        self.recordEditMode()
        f = self.current_pane
        for i in range(len(self.panes[f].lines)):
            text = self.getLineText(f, i)
            s = convert_to_format(text, format)
            # only modify lines that actually change
            if s != text:
                self.updateText(f, i, s)
        self.setFormat(f, format)

    # 'convert_to_dos' action
    def convert_to_dos(self):
        self.convert_format(DOS_FORMAT)

    # 'convert_to_mac' action
    def convert_to_mac(self):
        self.convert_format(MAC_FORMAT)

    # 'convert_to_unix' action
    def convert_to_unix(self):
        self.convert_format(UNIX_FORMAT)

    # copies the selected range of lines from pane 'f_src' to 'f_dst'
    def merge_lines(self, f_dst, f_src):
        self.recordEditMode()
        self.setLineMode()
        pane = self.panes[f_dst]
        start, end = self.selection_line, self.current_line
        if end < start:
            start, end = end, start
        end = min(end + 1, len(pane.lines))
        ss = [ self.getLineText(f_src, i) for i in range(start, end) ]
        if pane.format == 0:
            # copy the format of the source pane if the format for the
            # destination pane as not yet been determined
            self.setFormat(f_dst, getFormat(ss))
        for i, s in enumerate(ss):
            self.updateText(f_dst, start + i, convert_to_format(s, pane.format))
        n = len(ss)
        delta = min(self.removeSpacerLines(start, n), n - 1)
        if self.selection_line > start:
            self.selection_line -= delta
        if self.current_line > start:
            self.current_line -= delta
        self.recordEditMode()

    # callback for merge lines menu item
    def merge_lines_cb(self, widget, data):
        self.openUndoBlock()
        self.merge_lines(data, self.current_pane)
        self.closeUndoBlock()

    # 'copy_selection_right' action
    def copy_selection_right(self):
        f = self.current_pane + 1
        if f > 0 and f < len(self.panes):
            self.merge_lines(f, f - 1)

    # 'copy_selection_left' action
    def copy_selection_left(self):
        f = self.current_pane - 1
        if f >= 0 and f + 1 < len(self.panes):
            self.merge_lines(f, f + 1)

    # 'copy_left_into_selection' action
    def copy_left_into_selection(self):
        f = self.current_pane
        if f > 0 and f < len(self.panes):
            self.merge_lines(f, f - 1)

    # 'copy_right_into_selection' action
    def copy_right_into_selection(self):
        f = self.current_pane
        if f >= 0 and f + 1 < len(self.panes):
            self.merge_lines(f, f + 1)

    # merge from both left and right into the current pane
    def _mergeBoth(self, right_first):
        self.recordEditMode()
        self.setLineMode()
        f = self.current_pane
        start, end = self.selection_line, self.current_line
        if end < start:
            start, end = end, start
        end += 1
        npanes = len(self.panes)
        nlines = len(self.panes[f].lines)
        end = min(end, nlines)
        n = end - start
        if n > 0:
            lines = [ pane.lines[start:end] for pane in self.panes ]
            spaces = [ n * [ None ] for pane in self.panes ]
            old_content = [ line for line in lines[f] if line is not None ]
            for i in range(f + 1, npanes):
                lines[i], spaces[i] = spaces[i], lines[i]
            # replace f's lines with merge content
            if f > 0:
                lines[f] = lines[f - 1][:]
            else:
                lines[f] = n * [ None ]
            if f + 1 < npanes:
                spaces[f] = spaces[f + 1][:]
            else:
                spaces[f] = n * [ None ]
            if right_first:
                lines, spaces = spaces, lines

            pre, post = cutBlocks(end, self.blocks)
            pre, b = cutBlocks(start, pre)

            #  join and remove null lines
            b.extend(b)
            for l, s in zip(lines, spaces):
                l.extend(s)
            removeNullLines(b, lines)

            # replace f's lines with original, growing if necessary
            new_content = lines[f]
            new_n = len(new_content)
            lines[f] = old_content
            min_n = len(old_content)

            delta = new_n - min_n
            if delta < 0:
                delta = -delta
                for i in range(npanes):
                    if i != f:
                        lines[i].extend(delta * [ None ])
                # grow last block
                if len(b) > 0:
                    b[-1] += delta
                else:
                    b = createBlock(delta)
            elif delta > 0:
                old_content.extend(delta * [ None ])
                new_n = len(old_content)

            # update lines and blocks
            self.updateAlignment(start, n, lines)
            pre.extend(b)
            pre.extend(post)
            self.updateBlocks(pre)

            for i in range(new_n):
                s = None
                if i < len(new_content):
                    line = new_content[i]
                    if line is not None:
                        s = line.getText()
                self.updateText(f, start + i, s)

            # update selection
            end = start + new_n
            if end > start:
                end -= 1
            self.setCurrentLine(f, end, start)
        self.recordEditMode()

    # 'merge_from_left_then_right' keybinding action
    def merge_from_left_then_right(self):
        self._mergeBoth(False)

    # 'merge_from_right_then_left' keybinding action
    def merge_from_right_then_left(self):
        self._mergeBoth(True)

# create 'title_changed' signal for FileDiffViewer
gobject.signal_new('swapped_panes', FileDiffViewer, gobject.SIGNAL_RUN_LAST, gobject.TYPE_NONE, (int, int))
gobject.signal_new('num_edits_changed', FileDiffViewer, gobject.SIGNAL_RUN_LAST, gobject.TYPE_NONE, (int, ))
gobject.signal_new('mode_changed', FileDiffViewer, gobject.SIGNAL_RUN_LAST, gobject.TYPE_NONE, ())
gobject.signal_new('cursor_changed', FileDiffViewer, gobject.SIGNAL_RUN_LAST, gobject.TYPE_NONE, ())
gobject.signal_new('syntax_changed', FileDiffViewer, gobject.SIGNAL_RUN_LAST, gobject.TYPE_NONE, (str, ))
gobject.signal_new('format_changed', FileDiffViewer, gobject.SIGNAL_RUN_LAST, gobject.TYPE_NONE, (int, int))

# dialogue used to search for text
class SearchDialog(gtk.Dialog):
    def __init__(self, parent, pattern=None, history=None):
        gtk.Dialog.__init__(self, _('Find...'), parent, gtk.DIALOG_MODAL | gtk.DIALOG_DESTROY_WITH_PARENT, (gtk.STOCK_CANCEL, gtk.RESPONSE_REJECT, gtk.STOCK_OK, gtk.RESPONSE_ACCEPT))

        vbox = gtk.VBox()
        vbox.set_border_width(10)

        hbox = gtk.HBox()
        label = gtk.Label(_('Search For: '))
        hbox.pack_start(label, False, False, 0)
        label.show()
        combo = gtk.combo_box_entry_new_text()
        self.entry = combo.child
        self.entry.connect('activate', self.entry_cb)

        if pattern is not None:
            self.entry.set_text(pattern)

        if history is not None:
            completion = gtk.EntryCompletion()
            liststore = gtk.ListStore(gobject.TYPE_STRING)
            completion.set_model(liststore)
            completion.set_text_column(0)
            for h in history:
                liststore.append([h])
                combo.append_text(h)
            self.entry.set_completion(completion)

        hbox.pack_start(combo, True, True, 0)
        combo.show()
        vbox.pack_start(hbox, False, False, 0)
        hbox.show()

        button = gtk.CheckButton(_('Match Case'))
        self.match_case_button = button
        vbox.pack_start(button, False, False, 0)
        button.show()

        button = gtk.CheckButton(_('Search Backwards'))
        self.backwards_button = button
        vbox.pack_start(button, False, False, 0)
        button.show()

        self.vbox.pack_start(vbox, False, False, 0)
        vbox.show()

    # callback used when the Enter key is pressed
    def entry_cb(self, widget):
        self.response(gtk.RESPONSE_ACCEPT)

# convenience method to request confirmation when closing the last tab
def confirmTabClose(parent):
    dialog = MessageDialog(parent, gtk.MESSAGE_WARNING, _('Closing this tab will quit %s.') % (APP_NAME, ))
    end = (dialog.run() == gtk.RESPONSE_OK)
    dialog.destroy()
    return end

# custom dialogue for picking files with widgets for specifying the encoding
# and revision
class FileChooserDialog(gtk.FileChooserDialog):
    # record last chosen folder so the file chooser can start at a more useful
    # location for empty panes
    last_chosen_folder = os.path.realpath(os.curdir)

    def __current_folder_changed_cb(self, widget):
        FileChooserDialog.last_chosen_folder = widget.get_current_folder()

    def __init__(self, title, parent, prefs, action, accept, rev=False):
        gtk.FileChooserDialog.__init__(self, title, parent, action, (gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL, accept, gtk.RESPONSE_OK))
        self.prefs = prefs
        hbox = gtk.HBox()
        label = gtk.Label(_('Encoding: '))
        hbox.pack_start(label, False, False, 0)
        label.show()
        self.encoding = entry = EncodingMenu(prefs, action in [ gtk.FILE_CHOOSER_ACTION_OPEN, gtk.FILE_CHOOSER_ACTION_SELECT_FOLDER ])
        hbox.pack_start(entry, False, False, 5)
        entry.show()
        if rev:
            self.revision = entry = gtk.Entry()
            hbox.pack_end(entry, False, False, 0)
            entry.show()
            label = gtk.Label(_('Revision: '))
            hbox.pack_end(label, False, False, 0)
            label.show()

        self.vbox.pack_start(hbox, False, False, 0)
        hbox.show()
        self.set_current_folder(unicode(self.last_chosen_folder, sys.getfilesystemencoding()))
        self.connect('current-folder-changed', self.__current_folder_changed_cb)

    def set_encoding(self, encoding):
        self.encoding.set_text(encoding)

    def get_encoding(self):
        return self.encoding.get_text()

    def get_revision(self):
        return self.revision.get_text()

    def get_filename(self):
        # convert from UTF-8 string to unicode
        s = gtk.FileChooserDialog.get_filename(self)
        if s is not None:
            return unicode(s, 'utf_8')

# dialogue used to search for text
class NumericDialog(gtk.Dialog):
    def __init__(self, parent, title, text, val, lower, upper, step=1, page=0):
        gtk.Dialog.__init__(self, title, parent, gtk.DIALOG_MODAL | gtk.DIALOG_DESTROY_WITH_PARENT, (gtk.STOCK_CANCEL, gtk.RESPONSE_REJECT, gtk.STOCK_OK, gtk.RESPONSE_ACCEPT))

        vbox = gtk.VBox()
        vbox.set_border_width(10)

        hbox = gtk.HBox()
        label = gtk.Label(text)
        hbox.pack_start(label, False, False, 0)
        label.show()
        adj = gtk.Adjustment(val, lower, upper, step, page)
        self.button = button = gtk.SpinButton(adj, 0)
        button.connect('activate', self.button_cb)
        hbox.pack_start(button, True, True, 0)
        button.show()

        vbox.pack_start(hbox, True, True, 0)
        hbox.show()

        self.vbox.pack_start(vbox, False, False, 0)
        vbox.show()

    def button_cb(self, widget):
        self.response(gtk.RESPONSE_ACCEPT)

# establish callback for the about dialog's link to Diffuse's web site
def url_hook(dialog, link, userdata):
    webbrowser.open(link)

gtk.about_dialog_set_url_hook(url_hook, None)

class Diffuse(gtk.Window):
    class FileDiffViewer(FileDiffViewer):
        # pane header
        class PaneHeader(gtk.HBox):
            def __init__(self):
                gtk.HBox.__init__(self)
                appendButtons(self, gtk.ICON_SIZE_MENU, [
                   [ gtk.STOCK_OPEN, self.button_cb, 'open', _('Open File...') ],
                   [ gtk.STOCK_REFRESH, self.button_cb, 'reload', _('Reload File') ],
                   [ gtk.STOCK_SAVE, self.button_cb, 'save', _('Save File') ],
                   [ gtk.STOCK_SAVE_AS, self.button_cb, 'save_as', _('Save File As...') ] ])

                self.label = label = gtk.Label()
                label.set_size_request(0, label.get_size_request()[1])
                label.set_selectable(True)
                self.pack_start(label, True, True, 0)
                label.show()

                # file's name and information about how to retrieve it from a
                # VCS
                self.info = FileInfo()
                self.has_edits = False
                self.updateTitle()

            # callback for buttons
            def button_cb(self, widget, s):
                self.emit(s)

            # creates an appropriate title for the pane header
            def updateTitle(self):
                ss = []
                info = self.info
                if info.label is not None:
                    # show the provided label instead of the file name
                    ss.append(info.label)
                else:
                    if info.name is not None:
                        ss.append(info.name)
                    if info.revision is not None:
                        ss.append('(' + info.revision + ')')
                if self.has_edits:
                    ss.append('*')
                s = ' '.join(ss)
                self.label.set_text(s)
                set_tooltip(self.label, s)
                self.emit('title_changed')

            # set num edits
            def setEdits(self, has_edits):
                if self.has_edits != has_edits:
                    self.has_edits = has_edits
                    self.updateTitle()

        class PaneFooter(gtk.HBox):
            # set the cursor label
            def updateCursor(self, viewer, f):
                if viewer.mode == CHAR_MODE and viewer.current_pane == f:
                    j = viewer.current_char
                    if j > 0:
                        text = viewer.getLineText(viewer.current_pane, viewer.current_line)[:j]
                        j = viewer.stringWidth(text)
                    s = _('Column %d') % (j, )
                else:
                    s = ''
                self.cursor.set_text(s)

            # set the format label
            def setFormat(self, s):
                v = []
                if s & DOS_FORMAT:
                    v.append('DOS')
                if s & MAC_FORMAT:
                    v.append('Mac')
                if s & UNIX_FORMAT:
                    v.append('Unix')
                self.format.set_text('/'.join(v))

        def __init__(self, n, prefs, title):
            FileDiffViewer.__init__(self, n, prefs)

            self.title = title
            self.status = ''

            self.headers = []
            self.footers = []
            for i in range(n):
                # pane header
                w = Diffuse.FileDiffViewer.PaneHeader()
                self.headers.append(w)
                self.attach(w, i, i + 1, 0, 1, gtk.FILL, gtk.FILL)
                w.connect('title_changed', self.title_changed_cb)
                w.connect('open', self.open_file_button_cb, i)
                w.connect('reload', self.reload_file_button_cb, i)
                w.connect('save', self.save_file_button_cb, i)
                w.connect('save_as', self.save_file_as_button_cb, i)
                w.show()

                # pane footer
                w = Diffuse.FileDiffViewer.PaneFooter()
                self.footers.append(w)
                self.attach(w, i, i + 1, 2, 3, gtk.FILL, gtk.FILL)
                w.show()

            self.connect('swapped_panes', self.swapped_panes_cb)
            self.connect('num_edits_changed', self.num_edits_changed_cb)
            self.connect('mode_changed', self.mode_changed_cb)
            self.connect('cursor_changed', self.cursor_changed_cb)
            self.connect('format_changed', self.format_changed_cb)

            for i, darea in enumerate(self.dareas):
                darea.drag_dest_set(gtk.DEST_DEFAULT_ALL, [ ('text/uri-list', 0, 0) ], gtk.gdk.ACTION_COPY)
                darea.connect('drag_data_received', self.drag_data_received_cb, i)
            # initialise status
            self.updateStatus()

        # convenience method to request confirmation before loading a file if
        # it will cause existing edits to be lost
        def loadFromInfo(self, f, info):
            if self.headers[f].has_edits:
                # warn users of any unsaved changes they might lose
                dialog = gtk.MessageDialog(self.get_toplevel(), gtk.DIALOG_MODAL | gtk.DIALOG_DESTROY_WITH_PARENT, gtk.MESSAGE_WARNING, gtk.BUTTONS_NONE, _('Save changes before loading the new file?'))
                dialog.set_title(APP_NAME)
                dialog.add_button(gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL)
                dialog.add_button(gtk.STOCK_NO, gtk.RESPONSE_REJECT)
                dialog.add_button(gtk.STOCK_YES, gtk.RESPONSE_OK)
                dialog.set_default_response(gtk.RESPONSE_CANCEL)
                response = dialog.run()
                dialog.destroy()
                if response == gtk.RESPONSE_OK:
                    # save the current pane contents
                    if not self.save_file(f):
                        # cancel if the save failed
                        return
                elif response != gtk.RESPONSE_REJECT:
                    # cancel if the user did not choose 'yes' or 'no'
                    return
            self.openUndoBlock()
            self.recordEditMode()
            self.load(f, info)
            self.recordEditMode()
            self.closeUndoBlock()

        # callback used when receiving drag-n-drop data
        def drag_data_received_cb(self, widget, context, x, y, selection, targettype, eventtime, f):
            # parse uri-list
            s = selection.data
            ss, i, n = [], 0, len(s)
            while i < n:
                # skip separators
                while i < n:
                    v = ord(s[i])
                    if v > 32 and v < 127:
                        break
                    i += 1
                start = i
                # find end of item
                while i < n:
                    v = ord(s[i])
                    if v <= 32 or v >= 127:
                        break
                    i += 1
                if i > start:
                    p = url2path(s[start:i])
                    if p is not None:
                        ss.append(unicode(p, 'utf_8'))
            # load the first valid file
            for s in ss:
                if os.path.isfile(s):
                    self.loadFromInfo(f, FileInfo(s))
                    break

        # change the file info for pane 'f' to 'info'
        def setFileInfo(self, f, info):
            h, footer = self.headers[f], self.footers[f]
            h.info = info
            h.updateTitle()
            footer.setFormat(self.panes[f].format)
            footer.setEncoding(info.encoding)
            footer.updateCursor(self, f)

        # callback used when a pane header's title changes
        def title_changed_cb(self, widget):
            # choose a short but descriptive title for the viewer
            has_edits = False
            names = []
            unique_names = set()
            for header in self.headers:
                has_edits |= header.has_edits
                s = header.info.label
                if s is None:
                    # no label provided, show the file name instead
                    s = header.info.name
                    if s is not None:
                        s = os.path.basename(s)
                if s is not None:
                    names.append(s)
                    unique_names.add(s)

            if len(unique_names) > 0:
                if len(unique_names) == 1:
                    self.title = names[0]
                else:
                    self.title = ' : '.join(names)
            s = self.title
            if has_edits:
                s += ' *'
            self.emit('title_changed', s)

        def setEncoding(self, f, encoding):
            h = self.headers[f]
            h.info.encoding = encoding
            self.footers[f].setEncoding(encoding)

        # load a new file into pane 'f'
        # 'info' indicates the name of the file and how to retrieve it from the
        # version control system if applicable
        def load(self, f, info):
            name = info.name
            encoding = info.encoding
            stat = None
            if name is None:
                # reset to an empty pane
                ss = []
            else:
                rev = info.revision
                try:
                    if rev is None:
                        # load the contents of a plain file
                        fd = open(name, 'rb')
                        s = fd.read()
                        fd.close()
                        # get the file's modification times so we can detect changes
                        stat = os.stat(name)
                    else:
                        if info.vcs is None:
                            raise IOError('Not under version control.')
                        fullname = os.path.abspath(name)
                        # retrieve the revision from the version control system
                        s = info.vcs.getRevision(self.prefs, fullname, rev)
                    # convert file contents to unicode
                    if encoding is None:
                        s, encoding = self.prefs.convertToUnicode(s)
                    else:
                        s = unicode(s, encoding)
                    ss = splitlines(s)
                except (IOError, OSError, UnicodeDecodeError, WindowsError, LookupError):
                    # FIXME: this can occur before the toplevel window is drawn
                    if rev is not None:
                        msg = _('Error reading revision %(rev)s of %(file)s.') % { 'rev': rev, 'file': name }
                    else:
                        msg = _('Error reading %s.') % (name, )
                    dialog = MessageDialog(self.get_toplevel(), gtk.MESSAGE_ERROR, msg)
                    dialog.run()
                    dialog.destroy()
                    return
            # update the panes contents, last modified time, and title
            self.replaceContents(f, ss)
            info.encoding = encoding
            info.last_stat = info.stat = stat
            self.setFileInfo(f, info)
            # use the file name to choose appropriate syntax highlighting rules
            if name is not None:
                syntax = theResources.guessSyntaxForFile(name, ss)
                if syntax is not None:
                    self.setSyntax(syntax)

        # load a new file into pane 'f'
        def open_file(self, f, reload=False):
            h = self.headers[f]
            info = h.info
            if not reload:
                # we need to ask for a file name if we are not reloading the
                # existing file
                dialog = FileChooserDialog(_('Open File'), self.get_toplevel(), self.prefs, gtk.FILE_CHOOSER_ACTION_OPEN, gtk.STOCK_OPEN, True)
                if info.name is not None:
                    dialog.set_filename(os.path.realpath(info.name))
                dialog.set_encoding(info.encoding)
                dialog.set_default_response(gtk.RESPONSE_OK)
                end = (dialog.run() != gtk.RESPONSE_OK)
                name = dialog.get_filename()
                rev = None
                vcs = None
                revision = dialog.get_revision().strip()
                if revision != '':
                    rev = revision
                    vcs = theVCSs.findByFilename(name, self.prefs)
                info = FileInfo(name, dialog.get_encoding(), vcs, rev)
                dialog.destroy()
                if end:
                    return
            self.loadFromInfo(f, info)

        # callback for open file button
        def open_file_button_cb(self, widget, f):
            self.open_file(f)

        # callback for open file menu item
        def open_file_cb(self, widget, data):
            self.open_file(self.current_pane)

        # callback for reload file button
        def reload_file_button_cb(self, widget, f):
            self.open_file(f, True)

        # callback for reload file menu item
        def reload_file_cb(self, widget, data):
            self.open_file(self.current_pane, True)

        # check changes to files on disk when receiving keyboard focus
        def focus_in(self, widget, event):
            for f, h in enumerate(self.headers):
                info = h.info
                try:
                    if info.last_stat is not None:
                        info = h.info
                        new_stat = os.stat(info.name)
                        if info.last_stat[stat.ST_MTIME] < new_stat[stat.ST_MTIME]:
                            # update our notion of the most recent modification
                            info.last_stat = new_stat
                            if info.label is not None:
                                s = info.label
                            else:
                                s = info.name
                            msg = _('The file %s changed on disk.  Do you want to reload the file?') % (s, )
                            dialog = MessageDialog(self.get_toplevel(), gtk.MESSAGE_QUESTION, msg)
                            ok = (dialog.run() == gtk.RESPONSE_OK)
                            dialog.destroy()
                            if ok:
                                self.open_file(f, True)
                except OSError:
                    pass

        # save contents of pane 'f' to file
        def save_file(self, f, save_as=False):
            h = self.headers[f]
            info = h.info
            name, encoding, rev, label = info.name, info.encoding, info.revision, info.label
            if name is None or rev is not None:
                # we need to prompt for a file name the current contents were
                # not loaded from a regular file
                save_as = True
            if save_as:
                # prompt for a file name
                dialog = FileChooserDialog(_('Save %(title)s Pane %(pane)d') % { 'title': self.title, 'pane': f + 1 }, self.get_toplevel(), self.prefs, gtk.FILE_CHOOSER_ACTION_SAVE, gtk.STOCK_SAVE)
                if name is not None:
                    dialog.set_filename(os.path.abspath(name))
                if encoding is None:
                    encoding = self.prefs.getDefaultEncoding()
                dialog.set_encoding(encoding)
                name, label = None, None
                dialog.set_default_response(gtk.RESPONSE_OK)
                if dialog.run() == gtk.RESPONSE_OK:
                    name = dialog.get_filename()
                    encoding = dialog.get_encoding()
                    if encoding is None:
                        if info.encoding is not None:
                            # this case can occur if the user provided the
                            # encoding and it is not an encoding we know about
                            encoding = info.encoding
                        else:
                            encoding = self.prefs.getDefaultEncoding()
                dialog.destroy()
            if name is None:
                return False
            try:
                msg = None
                # warn if we are about to overwrite an existing file
                if save_as:
                    if os.path.exists(name):
                        msg = _('A file named %s already exists.  Do you want to overwrite it?') % (name, )
                # warn if we are about to overwrite a file that has changed
                # since we last read it
                elif info.stat is not None:
                    if info.stat[stat.ST_MTIME] < os.stat(name)[stat.ST_MTIME]:
                        msg = _('The file %s has been modified by another process since reading it.  If you save, all the external changes could be lost.  Save anyways?') % (name, )
                if msg is not None:
                    dialog = MessageDialog(self.get_toplevel(), gtk.MESSAGE_QUESTION, msg)
                    end = (dialog.run() != gtk.RESPONSE_OK)
                    dialog.destroy()
                    if end:
                        return False
            except OSError:
                pass
            try:
                # convert the text to the output encoding
                # refresh the lines to contain new objects with updated line
                # numbers and no local edits
                ss = []
                for line in self.panes[f].lines:
                    if line is not None:
                        s = line.getText()
                        if s is not None:
                            ss.append(s)
                encoded = codecs.encode(u''.join(ss), encoding)

                # write file
                fd = open(name, 'wb')
                fd.write(encoded)
                fd.close()

                # make the edits look permanent
                self.openUndoBlock()
                self.bakeEdits(f)
                self.closeUndoBlock()
                # update the pane file info
                info.name, info.encoding, info.revision, info.label = name, encoding, None, label
                info.last_stat = info.stat = os.stat(name)
                self.setFileInfo(f, info)
                # update the syntax highlighting incase we changed the file
                # extension
                syntax = theResources.guessSyntaxForFile(name, ss)
                if syntax is not None:
                    self.setSyntax(syntax)
                return True
            except (UnicodeEncodeError, LookupError):
                dialog = MessageDialog(self.get_toplevel(), gtk.MESSAGE_ERROR, _('Error encoding to %s.') % (encoding, ))
                dialog.run()
                dialog.destroy()
            except IOError:
                dialog = MessageDialog(self.get_toplevel(), gtk.MESSAGE_ERROR, _('Error writing %s.') % (name, ))
                dialog.run()
                dialog.destroy()
            return False

        # callback for save file menu item
        def save_file_cb(self, widget, data):
            self.save_file(self.current_pane)

        # callback for save file as menu item
        def save_file_as_cb(self, widget, data):
            self.save_file(self.current_pane, True)

        # callback for save all menu item
        def save_all_cb(self, widget, data):
            for f, h in enumerate(self.headers):
                if h.has_edits:
                    self.save_file(f)

        # callback for save file button
        def save_file_button_cb(self, widget, f):
            self.save_file(f)

        # callback for save file as button
        def save_file_as_button_cb(self, widget, f):
            self.save_file(f, True)

        # callback for go to line menu item
        def go_to_line_cb(self, widget, data):
            parent = self.get_toplevel()
            dialog = NumericDialog(parent, _('Go To Line...'), _('Line Number: '), 1, 1, self.panes[self.current_pane].max_line_number + 1)
            okay = (dialog.run() == gtk.RESPONSE_ACCEPT)
            i = dialog.button.get_value_as_int()
            dialog.destroy()
            if okay:
                self.go_to_line(i)

        # callback to receive notification when the name of a file changes
        def swapped_panes_cb(self, widget, f_dst, f_src):
            f0, f1 = self.headers[f_dst], self.headers[f_src]
            f0.has_edits, f1.has_edits = f1.has_edits, f0.has_edits
            info0, info1 = f1.info, f0.info
            self.setFileInfo(f_dst, info0)
            self.setFileInfo(f_src, info1)

        # callback to receive notification when the name of a file changes
        def num_edits_changed_cb(self, widget, f):
            self.headers[f].setEdits(self.panes[f].num_edits > 0)

        # callback to record changes to the viewer's mode
        def mode_changed_cb(self, widget):
            self.updateStatus()

        # update the viewer's current status message
        def updateStatus(self):
            if self.mode == LINE_MODE:
                s = _('Press the enter key or double click to edit.  Press the space bar or use the RMB menu to manually align.')
            elif self.mode == CHAR_MODE:
                s = _('Press the escape key to finish editing.')
            elif self.mode == ALIGN_MODE:
                s = _('Select target line and press the space bar to align.  Press the escape key to cancel.')
            else:
                s = None
            self.status = s
            self.emit('status_changed', s)

        # gets the status bar text
        def getStatus(self):
            return self.status

        # callback to display the cursor in a pane
        def cursor_changed_cb(self, widget):
            for f, footer in enumerate(self.footers):
                footer.updateCursor(self, f)

        # callback to display the format of a pane
        def format_changed_cb(self, widget, f, format):
            self.footers[f].setFormat(format)

    def __init__(self, rc_dir):
        menuspecs = []
        menuspecs.append([ _('_File'), [
                     [_('_Open File...'), self.open_file_cb, None, gtk.STOCK_OPEN, 'open_file'],
                     [_('Open File In New _Tab...'), self.open_file_in_new_tab_cb, None, None, 'open_file_in_new_tab'],
                     [_('Open _Modified Files...'), self.open_modified_files_cb, None, None, 'open_modified_files'],
                     [_('Open Commi_t...'), self.open_commit_cb, None, None, 'open_commit'],
                     [_('_Reload File'), self.reload_file_cb, None, gtk.STOCK_REFRESH, 'reload_file'],
                     [],
                     [_('_Save File'), self.save_file_cb, None, gtk.STOCK_SAVE, 'save_file'],
                     [_('Save File _As...'), self.save_file_as_cb, None, gtk.STOCK_SAVE_AS, 'save_file_as'],
                     [_('Save A_ll'), self.save_all_cb, None, None, 'save_all'],
                     [],
                     [_('New _2-Way File Merge'), self.new_2_way_file_merge_cb, None, DIFFUSE_STOCK_NEW_2WAY_MERGE, 'new_2_way_file_merge'],
                     [_('New _3-Way File Merge'), self.new_3_way_file_merge_cb, None, DIFFUSE_STOCK_NEW_3WAY_MERGE, 'new_3_way_file_merge'],
                     [_('New _N-Way File Merge...'), self.new_n_way_file_merge_cb, None, None, 'new_n_way_file_merge'],
                     [],
                     [_('_Close Tab'), self.close_tab_cb, None, gtk.STOCK_CLOSE, 'close_tab'],
                     [_('_Undo Close Tab'), self.undo_close_tab_cb, None, None, 'undo_close_tab'],
                     [_('_Quit'), self.quit_cb, None, gtk.STOCK_QUIT, 'quit'] ] ])

        menuspecs.append([ _('_Edit'), [
                     [_('_Undo'), self.button_cb, 'undo', gtk.STOCK_UNDO, 'undo'],
                     [_('_Redo'), self.button_cb, 'redo', gtk.STOCK_REDO, 'redo'],
                     [],
                     [_('Cu_t'), self.button_cb, 'cut', gtk.STOCK_CUT, 'cut'],
                     [_('_Copy'), self.button_cb, 'copy', gtk.STOCK_COPY, 'copy'],
                     [_('_Paste'), self.button_cb, 'paste', gtk.STOCK_PASTE, 'paste'],
                     [],
                     [_('Select _All'), self.button_cb, 'select_all', None, 'select_all'],
                     [_('C_lear Edits'), self.button_cb, 'clear_edits', gtk.STOCK_CLEAR, 'clear_edits'],
                     [_('_Dismiss All Edits'), self.button_cb, 'dismiss_all_edits', None, 'dismiss_all_edits'],
                     [],
                     [_('_Find...'), self.find_cb, None, gtk.STOCK_FIND, 'find'],
                     [_('Find _Next'), self.find_next_cb, None, None, 'find_next'],
                     [_('Find Pre_vious'), self.find_previous_cb, None, None, 'find_previous'],
                     [_('_Go To Line...'), self.go_to_line_cb, None, gtk.STOCK_JUMP_TO, 'go_to_line'],
                     [],
                     [_('Pr_eferences...'), self.preferences_cb, None, gtk.STOCK_PREFERENCES, 'preferences'] ] ])

        submenudef = [ [_('None'), self.syntax_cb, None, None, 'no_syntax_highlighting', True, None, ('syntax', None) ] ]
        names = theResources.getSyntaxNames()
        if len(names) > 0:
            submenudef.append([])
            names.sort(key=unicode.lower)
            for name in names:
                submenudef.append([name, self.syntax_cb, name, None, 'syntax_highlighting_' + name, True, None, ('syntax', name) ])

        menuspecs.append([ _('_View'), [
                     [_('_Syntax Highlighting'), None, None, None, None, True, submenudef],
                     [],
                     [_('Re_align All'), self.button_cb, 'realign_all', gtk.STOCK_EXECUTE, 'realign_all'],
                     [_('_Isolate'), self.button_cb, 'isolate', None, 'isolate'],
                     [],
                     [_('_First Difference'), self.button_cb, 'first_difference', gtk.STOCK_GOTO_TOP, 'first_difference'],
                     [_('_Previous Difference'), self.button_cb, 'previous_difference', gtk.STOCK_GO_UP, 'previous_difference'],
                     [_('_Next Difference'), self.button_cb, 'next_difference', gtk.STOCK_GO_DOWN, 'next_difference'],
                     [_('_Last Difference'), self.button_cb, 'last_difference', gtk.STOCK_GOTO_BOTTOM, 'last_difference'],
                     [],
                     [_('Fir_st Tab'), self.first_tab_cb, None, None, 'first_tab'],
                     [_('Pre_vious Tab'), self.previous_tab_cb, None, None, 'previous_tab'],
                     [_('Next _Tab'), self.next_tab_cb, None, None, 'next_tab'],
                     [_('Las_t Tab'), self.last_tab_cb, None, None, 'last_tab'],
                     [],
                     [_('Shift Pane _Right'), self.button_cb, 'shift_pane_right', None, 'shift_pane_right'],
                     [_('Shift Pane _Left'), self.button_cb, 'shift_pane_left', None, 'shift_pane_left'] ] ])

        menuspecs.append([ _('F_ormat'), [
                     [_('Convert To _Upper Case'), self.button_cb, 'convert_to_upper_case', None, 'convert_to_upper_case'],
                     [_('Convert To _Lower Case'), self.button_cb, 'convert_to_lower_case', None, 'convert_to_lower_case'],
                     [],
                     [_('Sort Lines In _Ascending Order'), self.button_cb, 'sort_lines_in_ascending_order', gtk.STOCK_SORT_ASCENDING, 'sort_lines_in_ascending_order'],
                     [_('Sort Lines In D_escending Order'), self.button_cb, 'sort_lines_in_descending_order', gtk.STOCK_SORT_DESCENDING, 'sort_lines_in_descending_order'],
                     [],
                     [_('Remove Trailing _White Space'), self.button_cb, 'remove_trailing_white_space', None, 'remove_trailing_white_space'],
                     [_('Convert Tabs To _Spaces'), self.button_cb, 'convert_tabs_to_spaces', None, 'convert_tabs_to_spaces'],
                     [_('Convert Leading Spaces To _Tabs'), self.button_cb, 'convert_leading_spaces_to_tabs', None, 'convert_leading_spaces_to_tabs'],
                     [],
                     [_('_Increase Indenting'), self.button_cb, 'increase_indenting', gtk.STOCK_INDENT, 'increase_indenting'],
                     [_('De_crease Indenting'), self.button_cb, 'decrease_indenting', gtk.STOCK_UNINDENT, 'decrease_indenting'],
                     [],
                     [_('Convert To _DOS Format'), self.button_cb, 'convert_to_dos', None, 'convert_to_dos'],
                     [_('Convert To _Mac Format'), self.button_cb, 'convert_to_mac', None, 'convert_to_mac'],
                     [_('Convert To Uni_x Format'), self.button_cb, 'convert_to_unix', None, 'convert_to_unix'] ] ])

        menuspecs.append([ _('_Merge'), [
                     [_('Copy Selection _Right'), self.button_cb, 'copy_selection_right', gtk.STOCK_GOTO_LAST, 'copy_selection_right'],
                     [_('Copy Selection _Left'), self.button_cb, 'copy_selection_left', gtk.STOCK_GOTO_FIRST, 'copy_selection_left'],
                     [],
                     [_('Copy Left _Into Selection'), self.button_cb, 'copy_left_into_selection', gtk.STOCK_GO_FORWARD, 'copy_left_into_selection'],
                     [_('Copy Right I_nto Selection'), self.button_cb, 'copy_right_into_selection', gtk.STOCK_GO_BACK, 'copy_right_into_selection'],
                     [_('_Merge From Left Then Right'), self.button_cb, 'merge_from_left_then_right', DIFFUSE_STOCK_LEFT_RIGHT, 'merge_from_left_then_right'],
                     [_('M_erge From Right Then Left'), self.button_cb, 'merge_from_right_then_left', DIFFUSE_STOCK_RIGHT_LEFT, 'merge_from_right_then_left'] ] ])

        menuspecs.append([ _('_Help'), [
                     [_('_Help Contents...'), self.help_contents_cb, None, gtk.STOCK_HELP, 'help_contents'],
                     [],
                     [_('_About %s...') % (APP_NAME, ), self.about_cb, None, gtk.STOCK_ABOUT, 'about'] ] ])

        # used to disable menu events when switching tabs
        self.menu_update_depth = 0
        # build list of radio menu items so we can update them to match the
        # currently viewed pane
        self.radio_menus = radio_menus = {}
        menu_bar = createMenuBar(menuspecs, radio_menus, accel_group)
        vbox.pack_start(menu_bar, False, False, 0)
        menu_bar.show()

        # create button bar
        hbox = gtk.HBox()
        appendButtons(hbox, gtk.ICON_SIZE_LARGE_TOOLBAR, [
           [ DIFFUSE_STOCK_NEW_2WAY_MERGE, self.new_2_way_file_merge_cb, None, _('New 2-Way File Merge') ],
           [ DIFFUSE_STOCK_NEW_3WAY_MERGE, self.new_3_way_file_merge_cb, None, _('New 3-Way File Merge') ],
           [],
           [ gtk.STOCK_EXECUTE, self.button_cb, 'realign_all', _('Realign All') ],
           [ gtk.STOCK_GOTO_TOP, self.button_cb, 'first_difference', _('First Difference') ],
           [ gtk.STOCK_GO_UP, self.button_cb, 'previous_difference', _('Previous Difference') ],
           [ gtk.STOCK_GO_DOWN, self.button_cb, 'next_difference', _('Next Difference') ],
           [ gtk.STOCK_GOTO_BOTTOM, self.button_cb, 'last_difference', _('Last Difference') ],
           [],
           [ gtk.STOCK_GOTO_LAST, self.button_cb, 'copy_selection_right', _('Copy Selection Right') ],
           [ gtk.STOCK_GOTO_FIRST, self.button_cb, 'copy_selection_left', _('Copy Selection Left') ],
           [ gtk.STOCK_GO_FORWARD, self.button_cb, 'copy_left_into_selection', _('Copy Left Into Selection') ],
           [ gtk.STOCK_GO_BACK, self.button_cb, 'copy_right_into_selection', _('Copy Right Into Selection') ],
           [ DIFFUSE_STOCK_LEFT_RIGHT, self.button_cb, 'merge_from_left_then_right', _('Merge From Left Then Right') ],
           [ DIFFUSE_STOCK_RIGHT_LEFT, self.button_cb, 'merge_from_right_then_left', _('Merge From Right Then Left') ],
           [],
           [ gtk.STOCK_UNDO, self.button_cb, 'undo', _('Undo') ],
           [ gtk.STOCK_REDO, self.button_cb, 'redo', _('Redo') ],
           [ gtk.STOCK_CUT, self.button_cb, 'cut', _('Cut') ],
           [ gtk.STOCK_COPY, self.button_cb, 'copy', _('Copy') ],
           [ gtk.STOCK_PASTE, self.button_cb, 'paste', _('Paste') ],
           [ gtk.STOCK_CLEAR, self.button_cb, 'clear_edits', _('Clear Edits') ] ])
        # avoid the button bar from dictating the minimum window size
        hbox.set_size_request(0, hbox.get_size_request()[1])
        vbox.pack_start(hbox, False, False, 0)
        hbox.show()


    def focus_in_cb(self, widget, event):
        for i in range(self.notebook.get_n_pages()):
            self.notebook.get_nth_page(i).focus_in(widget, event)

    def loadState(self, statepath):
        if os.path.isfile(statepath):
            try:
                f = open(statepath, 'r')
                ss = readlines(f)
                f.close()
                for j, s in enumerate(ss):
                    try:
                        a = shlex_split(s, True)
                        if len(a) > 0:
                            if len(a) == 2 and a[0] in self.bool_state:
                                self.bool_state[a[0]] = (a[1] == 'True')
                            elif len(a) == 2 and a[0] in self.int_state:
                                self.int_state[a[0]] = int(a[1])
                            else:
                                raise ValueError()
                    except ValueError:
                        # this may happen if the state was written by a
                        # different version -- don't bother the user
                        logDebug('Error processing line %d of %s.' % (j + 1, statepath))
            except IOError:
                # bad $HOME value? -- don't bother the user
                logDebug('Error reading %s.' % (statepath, ))

        self.move(self.int_state['window_x'], self.int_state['window_y'])
        self.resize(self.int_state['window_width'], self.int_state['window_height'])
        if self.bool_state['window_maximized']:
            self.maximize()

    # select viewer for a newly selected file in the confirm close dialogue
    def __confirmClose_row_activated_cb(self, tree, path, col, model):
        self.notebook.set_current_page(self.notebook.page_num(model[path][3]))

    # toggle save state for a file listed in the confirm close dialogue
    def __confirmClose_toggle_cb(self, cell, path, model):
        model[path][0] = not model[path][0]

    # returns True if the list of viewers can be closed.  The user will be
    # given a chance to save any modified files before this method completes.
    def confirmCloseViewers(self, viewers):
        # make a list of modified files
        model = gtk.ListStore(gobject.TYPE_BOOLEAN, gobject.TYPE_STRING, gobject.TYPE_INT, gobject.TYPE_OBJECT)
        for v in viewers:
            for f, h in enumerate(v.headers):
                if h.has_edits:
                    model.append((True, v.title, f + 1, v))
        if len(model) == 0:
            # there are no modified files, the viewers can be closed
            return True

        # ask the user which files should be saved
        dialog = gtk.MessageDialog(self.get_toplevel(), gtk.DIALOG_MODAL | gtk.DIALOG_DESTROY_WITH_PARENT, gtk.MESSAGE_WARNING, gtk.BUTTONS_NONE, _('Some files have unsaved changes.  Select the files to save before closing.'))
        dialog.set_resizable(True)
        dialog.set_title(APP_NAME)
        # add list of files with unsaved changes
        sw = gtk.ScrolledWindow()
        sw.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
        treeview = gtk.TreeView(model)
        r = gtk.CellRendererToggle()
        r.connect('toggled', self.__confirmClose_toggle_cb, model)
        column = gtk.TreeViewColumn(None, r)
        column.add_attribute(r, 'active', 0)
        treeview.append_column(column)
        r = gtk.CellRendererText()
        column = gtk.TreeViewColumn(_('Tab'), r, text=1)
        column.set_resizable(True)
        column.set_expand(True)
        column.set_sort_column_id(1)
        treeview.append_column(column)
        column = gtk.TreeViewColumn(_('Pane'), r, text=2)
        column.set_resizable(True)
        column.set_sort_column_id(2)
        treeview.append_column(column)
        treeview.connect('row_activated', self.__confirmClose_row_activated_cb, model)
        sw.add(treeview)
        treeview.show()
        dialog.vbox.pack_start(sw, True, True, 0)
        sw.show()
        # add custom set of action buttons
        dialog.add_button(gtk.STOCK_CANCEL, gtk.RESPONSE_CANCEL)
        button = gtk.Button(_('Close _Without Saving'))
        dialog.add_action_widget(button, gtk.RESPONSE_REJECT)
        button.show()
        dialog.add_button(gtk.STOCK_SAVE, gtk.RESPONSE_OK)
        dialog.set_default_response(gtk.RESPONSE_CANCEL)
        response = dialog.run()
        dialog.destroy()
        if response == gtk.RESPONSE_OK:
            # save all checked files
            it = model.get_iter_first()
            while it:
                if model.get_value(it, 0):
                    f = model.get_value(it, 2) - 1
                    v = model.get_value(it, 3)
                    if not v.save_file(f):
                        # cancel if we failed to save a file
                        return False
                it = model.iter_next(it)
            return True
        # cancel if the user did not choose 'Close Without Saving' or 'Save'
        return response == gtk.RESPONSE_REJECT

    def remove_tab_cb(self, widget, data):
        nb = self.notebook
        if nb.get_n_pages() > 1:
            # warn about losing unsaved changes before removing a tab
            if self.confirmCloseViewers([ data ]):
                self.closed_tabs.append((nb.page_num(data), data, nb.get_tab_label(data)))
                nb.remove(data)
                nb.set_show_tabs(self.prefs.getBool('tabs_always_show') or nb.get_n_pages() > 1)
        elif not self.prefs.getBool('tabs_warn_before_quit') or confirmTabClose(self.get_toplevel()):
            self.quit_cb(widget, data)

    # callback for RMB menu on notebook tabs
    def notebooktab_pick_cb(self, widget, data):
        self.notebook.set_current_page(data)

    def notebooktab_button_press_cb(self, widget, event, data):
        if event.button == 2:
            # remove the tab on MMB
            self.remove_tab_cb(widget, data)
        elif event.button == 3:
            # create a popup to pick a tab for focus on RMB
            menu = gtk.Menu()
            nb = self.notebook
            for i in range(nb.get_n_pages()):
                viewer = nb.get_nth_page(i)
                item = gtk.MenuItem(nb.get_tab_label(viewer).get_text(), False)
                item.connect('activate', self.notebooktab_pick_cb, i)
                menu.append(item)
                item.show()
                if viewer is data:
                    menu.select_item(item)
            menu.popup(None, None, None, event.button, event.time)

    # update window's title
    def updateTitle(self, viewer):
        title = self.notebook.get_tab_label(viewer).get_text()
        self.set_title('%s - %s' % (title, APP_NAME))

    # update the message in the status bar
    def setStatus(self, s):
        sb = self.statusbar
        context = sb.get_context_id('Message')
        sb.pop(context)
        if s is None:
            s = ''
        sb.push(context, s)

    # update the label in the status bar
    def setSyntax(self, s):
        # update menu
        t = self.radio_menus.get('syntax', None)
        if t is not None:
            t = t[1]
            if s in t:
                self.menu_update_depth += 1
                t[s].set_active(True)
                self.menu_update_depth -= 1

    def switch_page_cb(self, widget, ptr, page_num):
        viewer = widget.get_nth_page(page_num)
        self.updateTitle(viewer)
        self.setStatus(viewer.getStatus())
        self.setSyntax(viewer.getSyntax())

    def title_changed_cb(self, widget, title):
        # update the label in the notebook's tab
        self.notebook.get_tab_label(widget).set_text(title)
        if widget is self.getCurrentViewer():
            self.updateTitle(widget)

    def status_changed_cb(self, widget, s):
        # update the label in the notebook's tab
        if widget is self.getCurrentViewer():
            self.setStatus(s)

    def syntax_changed_cb(self, widget, s):
        # update the label
        if widget is self.getCurrentViewer():
            self.setSyntax(s)

    def newLoadedFileDiffViewer(self, items):
        specs = []
        if len(items) == 0:
            for i in range(self.prefs.getInt('tabs_default_panes')):
                specs.append(FileInfo())
        elif len(items) == 1 and len(items[0][1]) == 1:
            # one file specified
            # determine which other files to compare it with
            name, data, label = items[0]
            rev, encoding = data[0]
            vcs = theVCSs.findByFilename(name, self.prefs)
            if vcs is None:
                # shift the existing file so it will be in the second pane
                specs.append(FileInfo())
                specs.append(FileInfo(name, encoding, None, None, label))
            else:
                if rev is None:
                    # no revision specified assume defaults
                    for name, rev in vcs.getFileTemplate(self.prefs, name):
                        if rev is None:
                            s = label
                        else:
                            s = None
                        specs.append(FileInfo(name, encoding, vcs, rev, s))
                else:
                    # single revision specified
                    specs.append(FileInfo(name, encoding, vcs, rev))
                    specs.append(FileInfo(name, encoding, None, None, label))
        else:
            # multiple files specified, use one pane for each file
            for name, data, label in items:
                for rev, encoding in data:
                    if rev is None:
                        vcs, s = None, label
                    else:
                        vcs, s = theVCSs.findByFilename(name, self.prefs), None
                    specs.append(FileInfo(name, encoding, vcs, rev, s))

        # open a new viewer
        viewer = self.newFileDiffViewer(max(2, len(specs)))

        # load the files
        for i, spec in enumerate(specs):
            viewer.load(i, spec)
        return viewer

    # create a new viewer for 'items'
    def createSingleTab(self, items, labels, options):
        if len(items) > 0:
            self.newLoadedFileDiffViewer(assign_file_labels(items, labels)).setOptions(options)

    # create a new viewer for each item in 'items'
    def createSeparateTabs(self, items, labels, options):
        # all tabs inherit the first tab's revision and encoding specifications
        items = [ (name, items[0][1]) for name, data in items ]
        for item in assign_file_labels(items, labels):
            self.newLoadedFileDiffViewer([ item ]).setOptions(options)

    # create a new viewer for each modified file found in 'items'
    def createCommitFileTabs(self, items, labels, options):
        new_items = []
        for item in items:
            name, data = item
            # get full path to an existing ancessor directory
            dn = os.path.abspath(name)
            while not os.path.isdir(dn):
                dn, old_dn = os.path.dirname(dn), dn
                if dn == old_dn:
                    break
            if len(new_items) == 0 or dn != new_items[-1][0]:
                new_items.append([ dn, None, [] ])
            dst = new_items[-1]
            dst[1] = data[-1][1]
            dst[2].append(name)
        for dn, encoding, names in new_items:
            vcs = theVCSs.findByFolder(dn, self.prefs)
            if vcs is not None:
                try:
                    for specs in vcs.getCommitTemplate(self.prefs, options['commit'], names):
                        viewer = self.newFileDiffViewer(len(specs))
                        for i, spec in enumerate(specs):
                            name, rev = spec
                            viewer.load(i, FileInfo(name, encoding, vcs, rev))
                        viewer.setOptions(options)
                except (IOError, OSError, WindowsError):
                    dialog = MessageDialog(self.get_toplevel(), gtk.MESSAGE_ERROR, _('Error retrieving commits for %s.') % (dn, ))
                    dialog.run()
                    dialog.destroy()

    # create a new viewer for each modified file found in 'items'
    def createModifiedFileTabs(self, items, labels, options):
        new_items = []
        for item in items:
            name, data = item
            # get full path to an existing ancessor directory
            dn = os.path.abspath(name)
            while not os.path.isdir(dn):
                dn, old_dn = os.path.dirname(dn), dn
                if dn == old_dn:
                    break
            if len(new_items) == 0 or dn != new_items[-1][0]:
                new_items.append([ dn, None, [] ])
            dst = new_items[-1]
            dst[1] = data[-1][1]
            dst[2].append(name)
        for dn, encoding, names in new_items:
            vcs = theVCSs.findByFolder(dn, self.prefs)
            if vcs is not None:
                try:
                    for specs in vcs.getFolderTemplate(self.prefs, names):
                        viewer = self.newFileDiffViewer(len(specs))
                        for i, spec in enumerate(specs):
                            name, rev = spec
                            viewer.load(i, FileInfo(name, encoding, vcs, rev))
                        viewer.setOptions(options)
                except (IOError, OSError, WindowsError):
                    dialog = MessageDialog(self.get_toplevel(), gtk.MESSAGE_ERROR, _('Error retrieving modifications for %s.') % (dn, ))
                    dialog.run()
                    dialog.destroy()

    # returns True if the application can safely quit
    def confirmQuit(self):
        nb = self.notebook
        return self.confirmCloseViewers([ nb.get_nth_page(i) for i in range(nb.get_n_pages()) ])

    # returns the currently focused viewer
    def getCurrentViewer(self):
        return self.notebook.get_nth_page(self.notebook.get_current_page())

    # callback for the open file menu item
    def open_file_cb(self, widget, data):
        self.getCurrentViewer().open_file_cb(widget, data)

    # callback for the open file menu item
    def open_file_in_new_tab_cb(self, widget, data):
        specs = None
        dialog = FileChooserDialog(_('Open File In New Tab'), self.get_toplevel(), self.prefs, gtk.FILE_CHOOSER_ACTION_OPEN, gtk.STOCK_OPEN, True)
        dialog.set_default_response(gtk.RESPONSE_OK)
        accept = (dialog.run() == gtk.RESPONSE_OK)
        name, encoding = dialog.get_filename(), dialog.get_encoding()
        rev = dialog.get_revision().strip()
        if rev == '':
            rev = None
        dialog.destroy()
        if accept:
            viewer = self.newLoadedFileDiffViewer([ (name, [ (rev, encoding) ], None) ])
            self.notebook.set_current_page(self.notebook.get_n_pages() - 1)
            viewer.grab_focus()

    # callback for the open modified files menu item
    def open_modified_files_cb(self, widget, data):
        parent = self.get_toplevel()
        dialog = FileChooserDialog(_('Choose Folder With Modified Files'), parent, self.prefs, gtk.FILE_CHOOSER_ACTION_SELECT_FOLDER, gtk.STOCK_OPEN)
        dialog.set_default_response(gtk.RESPONSE_OK)
        accept = (dialog.run() == gtk.RESPONSE_OK)
        name, encoding = dialog.get_filename(), dialog.get_encoding()
        dialog.destroy()
        if accept:
            n = self.notebook.get_n_pages()
            self.createModifiedFileTabs([ (name, [ (None, encoding) ]) ], [], {})
            if self.notebook.get_n_pages() > n:
                # we added some new tabs, focus on the first one
                self.notebook.set_current_page(n)
                self.getCurrentViewer().grab_focus()
            else:
                m = MessageDialog(parent, gtk.MESSAGE_ERROR, _('No modified files found.'))
                m.run()
                m.destroy()

    # callback for the open commit menu item
    def open_commit_cb(self, widget, data):
        parent = self.get_toplevel()
        dialog = FileChooserDialog(_('Choose Folder With Commit'), parent, self.prefs, gtk.FILE_CHOOSER_ACTION_SELECT_FOLDER, gtk.STOCK_OPEN, True)
        dialog.set_default_response(gtk.RESPONSE_OK)
        accept = (dialog.run() == gtk.RESPONSE_OK)
        name, rev, encoding = dialog.get_filename(), dialog.get_revision(), dialog.get_encoding()
        dialog.destroy()
        if accept:
            n = self.notebook.get_n_pages()
            self.createCommitFileTabs([ (name, [ (None, encoding) ]) ], [], { 'commit': rev })
            if self.notebook.get_n_pages() > n:
                # we added some new tabs, focus on the first one
                self.notebook.set_current_page(n)
                self.getCurrentViewer().grab_focus()
            else:
                m = MessageDialog(parent, gtk.MESSAGE_ERROR, _('No committed files found.'))
                m.run()
                m.destroy()

    # callback for the reload file menu item
    def reload_file_cb(self, widget, data):
        self.getCurrentViewer().reload_file_cb(widget, data)

    # callback for the save file menu item
    def save_file_cb(self, widget, data):
        self.getCurrentViewer().save_file_cb(widget, data)

    # callback for the save file as menu item
    def save_file_as_cb(self, widget, data):
        self.getCurrentViewer().save_file_as_cb(widget, data)

    # callback for the save all menu item
    def save_all_cb(self, widget, data):
        for i in range(self.notebook.get_n_pages()):
            self.notebook.get_nth_page(i).save_all_cb(widget, data)

    # callback for the new 2-way file merge menu item
    def new_2_way_file_merge_cb(self, widget, data):
        viewer = self.newFileDiffViewer(2)
        self.notebook.set_current_page(self.notebook.get_n_pages() - 1)
        viewer.grab_focus()

    # callback for the new 3-way file merge menu item
    def new_3_way_file_merge_cb(self, widget, data):
        viewer = self.newFileDiffViewer(3)
        self.notebook.set_current_page(self.notebook.get_n_pages() - 1)
        viewer.grab_focus()

    # callback for the new n-way file merge menu item
    def new_n_way_file_merge_cb(self, widget, data):
        parent = self.get_toplevel()
        dialog = NumericDialog(parent, _('New N-Way File Merge...'), _('Number of panes: '), 4, 2, 16)
        okay = (dialog.run() == gtk.RESPONSE_ACCEPT)
        npanes = dialog.button.get_value_as_int()
        dialog.destroy()
        if okay:
            viewer = self.newFileDiffViewer(npanes)
            self.notebook.set_current_page(self.notebook.get_n_pages() - 1)
            viewer.grab_focus()

    # callback for the close tab menu item
    def close_tab_cb(self, widget, data):
        self.remove_tab_cb(widget, self.notebook.get_nth_page(self.notebook.get_current_page()))

    # callback for the undo close tab menu item
    def undo_close_tab_cb(self, widget, data):
        if len(self.closed_tabs) > 0:
            i, tab, tab_label = self.closed_tabs.pop()
            self.notebook.insert_page(tab, tab_label, i)
            self.notebook.set_current_page(i)
            self.notebook.set_show_tabs(True)

    # callback for the quit menu item
    def quit_cb(self, widget, data):
        if self.confirmQuit():
            gtk.main_quit()

    # request search parameters if force=True and then perform a search in the
    # current viewer pane
    def find(self, force, reverse):
        viewer = self.getCurrentViewer()
        if force or self.search_pattern is None:
            # construct search dialog
            history = self.search_history
            pattern = viewer.getSelectedText()
            for c in '\r\n':
                i = pattern.find(c)
                if i >= 0:
                    pattern = pattern[:i]
            dialog = SearchDialog(self.get_toplevel(), pattern, history)
            dialog.match_case_button.set_active(self.bool_state['search_matchcase'])
            dialog.backwards_button.set_active(self.bool_state['search_backwards'])
            keep = (dialog.run() == gtk.RESPONSE_ACCEPT)
            # persist the search options
            pattern = dialog.entry.get_text()
            match_case = dialog.match_case_button.get_active()
            backwards = dialog.backwards_button.get_active()
            dialog.destroy()
            if not keep or pattern == '':
                return
            # perform the search
            self.search_pattern = pattern
            if pattern in history:
                del history[history.index(pattern)]
            history.insert(0, pattern)
            self.bool_state['search_matchcase'] = match_case
            self.bool_state['search_backwards'] = backwards

        # determine where to start searching from
        reverse ^= self.bool_state['search_backwards']
        from_start, more = False, True
        while more:
            if viewer.find(self.search_pattern, self.bool_state['search_matchcase'], reverse, from_start):
                break

            if reverse:
                msg = _('Phrase not found.  Continue from the end of the file?')
            else:
                msg = _('Phrase not found.  Continue from the start of the file?')
            dialog = MessageDialog(self.get_toplevel(), gtk.MESSAGE_QUESTION, msg)
            dialog.set_default_response(gtk.RESPONSE_OK)
            more = (dialog.run() == gtk.RESPONSE_OK)
            dialog.destroy()
            from_start = True

    # callback for the find menu item
    def find_cb(self, widget, data):
        self.find(True, False)

    # callback for the find next menu item
    def find_next_cb(self, widget, data):
        self.find(False, False)

    # callback for the find previous menu item
    def find_previous_cb(self, widget, data):
        self.find(False, True)

    # callback for the go to line menu item
    def go_to_line_cb(self, widget, data):
        self.getCurrentViewer().go_to_line_cb(widget, data)

    # notify all viewers of changes to the preferences
    def preferences_updated(self):
        n = self.notebook.get_n_pages()
        self.notebook.set_show_tabs(self.prefs.getBool('tabs_always_show') or n > 1)
        for i in range(n):
            self.notebook.get_nth_page(i).prefsUpdated()

    # callback for the preferences menu item
    def preferences_cb(self, widget, data):
        if self.prefs.runDialog(self.get_toplevel()):
            self.preferences_updated()

    # callback for all of the syntax highlighting menu items
    def syntax_cb(self, widget, data):
        # ignore events while we update the menu when switching tabs
        # also ignore notification of the newly disabled item
        if self.menu_update_depth == 0 and widget.get_active():
            self.getCurrentViewer().setSyntax(data)

    # callback for the first tab menu item
    def first_tab_cb(self, widget, data):
        self.notebook.set_current_page(0)

    # callback for the previous tab menu item
    def previous_tab_cb(self, widget, data):
        i, n = self.notebook.get_current_page(), self.notebook.get_n_pages()
        self.notebook.set_current_page((n + i - 1) % n)

    # callback for the next tab menu item
    def next_tab_cb(self, widget, data):
        i, n = self.notebook.get_current_page(), self.notebook.get_n_pages()
        self.notebook.set_current_page((i + 1) % n)

    # callback for the last tab menu item
    def last_tab_cb(self, widget, data):
        self.notebook.set_current_page(self.notebook.get_n_pages() - 1)

    # callback for most menu items and buttons
    def button_cb(self, widget, data):
        self.getCurrentViewer().button_cb(widget, data)

    # display help documenation
    def help_contents_cb(self, widget, data):
        help_url = None
        if isWindows():
            # help documentation is distributed as local HTML files
            # search for localised manual first
            parts = [ 'manual' ]
            if lang is not None:
                parts = [ 'manual' ]
                parts.extend(lang.split('_'))
            while len(parts) > 0:
                help_file = os.path.join(bin_dir, '_'.join(parts) + '.html')
                if os.path.isfile(help_file):
                    # we found a help file
                    help_url = path2url(help_file)
                    break
                del parts[-1]
        else:
            # verify gnome-help is available
            browser = None
            p = os.environ.get('PATH', None)
            if p is not None:
                for s in p.split(os.pathsep):
                    fp = os.path.join(s, 'gnome-help')
                    if os.path.isfile(fp):
                        browser = fp
                        break
            if browser is not None:
                # find localised help file
                if lang is None:
                    parts = []
                else:
                    parts = lang.split('_')
                s = os.path.abspath(os.path.join(bin_dir, '../share/gnome/help/diffuse'))
                while True:
                    if len(parts) > 0:
                        d = '_'.join(parts)
                    else:
                        # fall back to using 'C'
                        d = 'C'
                    help_file = os.path.join(os.path.join(s, d), 'diffuse.xml')
                    if os.path.isfile(help_file):
                        args = [ browser, path2url(help_file, 'ghelp') ]
                        # spawnvp is not available on some systems, use spawnv instead
                        os.spawnv(os.P_NOWAIT, args[0], args)
                        return
                    if len(parts) == 0:
                        break
                    del parts[-1]
        if help_url is None:
            # no local help file is available, show on-line help
            help_url = WEBSITE + 'manual.html'
            # ask for localised manual
            if lang is not None:
                help_url += '?lang=' + lang
        # use a web browser to display the help documentation
        webbrowser.open(help_url)

    # callback for the about menu item
    def about_cb(self, widget, data):
        dialog = AboutDialog()
        dialog.run()
        dialog.destroy()

gobject.signal_new('title_changed', Diffuse.FileDiffViewer, gobject.SIGNAL_RUN_LAST, gobject.TYPE_NONE, (str, ))
gobject.signal_new('status_changed', Diffuse.FileDiffViewer, gobject.SIGNAL_RUN_LAST, gobject.TYPE_NONE, (str, ))
gobject.signal_new('title_changed', Diffuse.FileDiffViewer.PaneHeader, gobject.SIGNAL_RUN_LAST, gobject.TYPE_NONE, ())
gobject.signal_new('open', Diffuse.FileDiffViewer.PaneHeader, gobject.SIGNAL_RUN_LAST, gobject.TYPE_NONE, ())
gobject.signal_new('reload', Diffuse.FileDiffViewer.PaneHeader, gobject.SIGNAL_RUN_LAST, gobject.TYPE_NONE, ())
gobject.signal_new('save', Diffuse.FileDiffViewer.PaneHeader, gobject.SIGNAL_RUN_LAST, gobject.TYPE_NONE, ())
gobject.signal_new('save_as', Diffuse.FileDiffViewer.PaneHeader, gobject.SIGNAL_RUN_LAST, gobject.TYPE_NONE, ())
 */

  // Find the user's config directory and create it, if it does not exist
  bool found = false;
  std::string rc_dir;
  std::vector<std::string> subdirs{"diffuse"};
  if (Df::isWindows()) {
    rc_dir = Glib::locale_from_utf8(
        Glib::getenv(Glib::locale_from_utf8("XDG_CONFIG_HOME"), found));
  } else {
    rc_dir = Glib::getenv(Glib::locale_from_utf8("XDG_CONFIG_HOME"), found);
  }
  if (!found || rc_dir.empty()) {
    rc_dir = Glib::get_home_dir();
    subdirs.emplace(std::cbegin(subdirs), ".config");
  }
  if (!make_subdirs(rc_dir, subdirs)) {
    std::cerr << "Failed to create user configuration directory\n";
    return 1;
  }
  // Find the user's data directory and create it, if it does not exist
  found = false;
  std::string data_dir;
  subdirs = {"diffuse"};
  if (Df::isWindows()) {
    data_dir = Glib::locale_from_utf8(
        Glib::getenv(Glib::locale_from_utf8("XDG_DATA_HOME"), found));
  } else {
    data_dir = Glib::getenv(Glib::locale_from_utf8("XDG_DATA_HOME"), found);
  }
  if (!found || data_dir.empty()) {
    data_dir = Glib::get_home_dir();
    subdirs.emplace(std::cbegin(subdirs), "share");
    subdirs.emplace(std::cbegin(subdirs), ".local");
  }
  if (!make_subdirs(data_dir, subdirs)) {
    std::cerr << "Failed to create user data directory\n";
    return 1;
  }

  // Process the commandline arguments

  // Load resource files
  auto i = 1u;
  std::vector<std::string> rc_files;
  if ((i < static_cast<unsigned>(argc)) && ("--no-rcfile" == args[i])) {
    ++i;
  } else if ((i + 1 < static_cast<unsigned>(argc)) && ("--rcfile" == args[i])) {
    ++i;
    rc_files.emplace_back(Glib::locale_from_utf8(args[i]));
    ++i;
  } else {
    std::string tmp_rc_file;
    // Parse system-wide, then personal initialization files
    if (Df::isWindows()) {
      tmp_rc_file =
          Glib::build_filename(bin_dir, Glib::locale_from_utf8("diffuserc"));
    } else {
      tmp_rc_file = Glib::build_filename(
          bin_dir, Glib::locale_from_utf8("../../etc/diffuserc"));
      gchar *canonicalized =
          g_canonicalize_filename(tmp_rc_file.data(), nullptr);
      tmp_rc_file = canonicalized;
      g_free(canonicalized);
      canonicalized = nullptr;
    }
    for (const auto &rc_file : std::vector<std::string>{
             tmp_rc_file, Glib::build_filename(
                              rc_dir, Glib::locale_from_utf8("diffuserc"))}) {
      if (Glib::file_test(rc_file, Glib::FileTest::FILE_TEST_IS_REGULAR)) {
        rc_files.emplace_back(rc_file);
      }
    }
  }
  for (const auto &rc_file : rc_files) {
    // Convert to an absolute path so that the locations of any processing
    // errors are reported with normalized file names
    gchar *tmp_rc_file = g_canonicalize_filename(rc_file.data(), nullptr);
    std::string rc_file_abspath{tmp_rc_file};
    g_free(tmp_rc_file);
    tmp_rc_file = nullptr;
    if (!theResources.parse(rc_file_abspath)) {
      Df::logError(Glib::ustring::compose(
          _("Error reading %1."), Glib::locale_to_utf8(rc_file_abspath)));
    }
  }

  Df::Diffuse diff{rc_dir};
  // Load state
  const auto statepath{
      Glib::build_filename(data_dir, Glib::locale_from_utf8("state"))};
  if (!diff.loadState(statepath)) {
    std::cerr << "Failed to load state information\n";
    return 1;
  }

  // Process remaining commandline arguments
  using namespace std::placeholders;
  bool close_on_same = false;
  Df::Diffuse::Encoding encoding;
  std::map<Glib::ustring, std::function<void(const Df::Diffuse::Specs &,
                                             const Df::Diffuse::Labels &,
                                             const Df::Options &)>>
      funcs{{"commit",
             std::bind(&Df::Diffuse::createCommitFileTabs, &diff, _1, _2, _3)},
            {"modified", std::bind(&Df::Diffuse::createModifiedFileTabs, &diff,
                                   _1, _2, _3)},
            {"separate",
             std::bind(&Df::Diffuse::createSeparateTabs, &diff, _1, _2, _3)},
            {"single",
             std::bind(&Df::Diffuse::createSingleTab, &diff, _1, _2, _3)}};
  bool had_specs = false;
  Df::Diffuse::Labels labels;
  Glib::ustring mode{"single"};
  Df::Options options;
  Df::Diffuse::Revisions revs;
  Df::Diffuse::Specs specs;

  while (i < static_cast<unsigned>(argc)) {
    const auto arg{args[i]};
    if ((!arg.empty()) && ('-' == arg[0])) {
      if ((i + 1 < static_cast<unsigned>(argc)) &&
          (("-c" == arg) || ("--commit" == arg))) {
        // Specified revision
        funcs[mode](specs, labels, options);
        ++i;
        const auto rev{args[i]};
        labels.clear();
        options = {{"commit", args[i]}};
        specs.clear();
        mode = "commit";
      } else if (("-D" == arg) || ("--close-if-same" == arg)) {
        close_on_same = true;
      } else if ((i + 1 < static_cast<unsigned>(argc)) &&
                 (("-e" == arg) || ("--encoding" == arg))) {
        ++i;
        encoding = args[i];
        // TODO: encoding = encodings.aliases.aliases.get(encoding, encoding)
      } else if (("-m" == arg) || ("--modified" == arg)) {
        funcs[mode](specs, labels, options);
        labels.clear();
        options.clear();
        specs.clear();
        mode = "modified";
      } else if ((i + 1 < static_cast<unsigned>(argc)) &&
                 (("-r" == arg) || ("--revision" == arg))) {
        // Specified revision
        ++i;
        revs.emplace_back(Df::Diffuse::Revision{args[i], encoding});
      } else if (("-s" == arg) || ("--separate" == arg)) {
        funcs[mode](specs, labels, options);
        labels.clear();
        options.clear();
        specs.clear();
        // Open items in separate tabs
        mode = "separate";
      } else if (("-t" == arg) || ("--tab" == arg)) {
        funcs[mode](specs, labels, options);
        labels.clear();
        options.clear();
        specs.clear();
        // Start a new tab
        mode = "single";
      } else if ((i + 1 < static_cast<unsigned>(argc)) &&
                 (("-V" == arg) || ("--vcs" == arg))) {
        ++i;
        diff.prefs.setString("vcs_search_order", args[i]);
        diff.preferences_updated();
      } else if (("-b" == arg) || ("--ignore-space-change" == arg)) {
        diff.prefs.setBool("display_ignore_whitespace_changes", true);
        diff.prefs.setBool("align_ignore_whitespace_changes", true);
        diff.preferences_updated();
      } else if (("-B" == arg) || ("--ignore-blank-lines" == arg)) {
        diff.prefs.setBool("display_ignore_blanklines", true);
        diff.prefs.setBool("align_ignore_blanklines", true);
        diff.preferences_updated();
      } else if (("-E" == arg) || ("--ignore-end-of-line" == arg)) {
        diff.prefs.setBool("display_ignore_endofline", true);
        diff.prefs.setBool("align_ignore_endofline", true);
        diff.preferences_updated();
      } else if (("-i" == arg) || ("--ignore-case" == arg)) {
        diff.prefs.setBool("display_ignore_case", true);
        diff.prefs.setBool("align_ignore_case", true);
        diff.preferences_updated();
      } else if (("-w" == arg) || ("--ignore-all-space" == arg)) {
        diff.prefs.setBool("display_ignore_whitespace", true);
        diff.prefs.setBool("align_ignore_whitespace", true);
        diff.preferences_updated();
      } else if ((i + 1 < static_cast<unsigned>(argc)) && ("-L" == arg)) {
        ++i;
        labels.emplace_back(args[i]);
      } else if ((i + 1 < static_cast<unsigned>(argc)) && ("--line" == arg)) {
        ++i;
        try {
          options["line"] = std::stoul(args[i]);
        } catch (const std::invalid_argument &) {
          Df::logError("Could not convert line argument to number");
        } catch (const std::out_of_range &) {
          Df::logError("Given line number is out of range");
        }
      } else if ("--null-file" == arg) {
        // Add a blank file pane
        if (("separate" == mode) || ("single" == mode)) {
          if (revs.empty()) {
            revs.emplace_back(Df::Diffuse::Revision{std::nullopt, encoding});
          }
          specs.emplace_back(Df::Diffuse::Specification{std::nullopt, revs});
          revs.clear();
        }
        had_specs = true;
      } else {
        Df::logError(Glib::ustring::compose(
            _("Skipping unknown argument \"%1\"."), args[i]));
      }
    } else {
      std::optional<std::string> filename{
          diff.prefs.convertToNativePath(args[i])};
      if ((("separate" == mode) || ("single" == mode)) &&
          Glib::file_test(filename.value(), Glib::FileTest::FILE_TEST_IS_DIR)) {
        if (!specs.empty()) {
          filename = Glib::build_filename(
              filename.value(),
              Glib::path_get_basename(specs.back().path.value()));
        } else {
          Df::logError(Glib::ustring::compose(
              _("Error processing argument \"%2\".  Directory not expected."),
              args[i]));
          filename.reset();
        }
      }
      if (filename) {
        if (revs.empty()) {
          revs.emplace_back(Df::Diffuse::Revision{std::nullopt, encoding});
        }
        specs.emplace_back(Df::Diffuse::Specification{filename, revs});
        revs.clear();
      }
      had_specs = true;
    }
    ++i;
  }
  if ((("commit" == mode) || ("modified" == mode)) && specs.empty()) {
    specs.emplace_back(Df::Diffuse::Specification{
        Glib::get_current_dir(),
        Df::Diffuse::Revisions{{std::nullopt, encoding}}});
    had_specs = true;
  }
  funcs[mode](specs, labels, options);

  // Create a file diff viewer if the commandline arguments have not created any
  // implicitly
  if (!had_specs) {
    diff.newLoadedFileDiffViewer({});
  } else if (close_on_same) {
    diff.closeOnSame();
  }

  auto &nb{diff.notebook};
  const auto n = nb.get_n_pages();
  if (0 < n) {
    nb.set_show_tabs(diff.prefs.getBool("tabs_always_show") || (1 < n));
    nb.get_nth_page(0)->grab_focus();
    diff.show();
    app.run();
    // Save state
    diff.saveState(statepath);
  }

  return 0;
}

static std::vector<Glib::ustring> convert_args(int argc, char *argv[]) {
  std::vector<Glib::ustring> res;

  for (auto i = 0; i < argc; ++i) {
    res.emplace_back(Glib::locale_to_utf8(argv[i]));
  }

  return res;
}

/**
 * @brief Create nested sub-directories within a path
 * @param[in,out] p The path to the directory in which the sub-directories shall
 *   be created (will contain the new path in case of success or if it exists
 *   already)
 * @param[in] ss The hierarchy of sub-directories to be created
 * @return _true_ in case of success or if the directory hierarchy exists
 *   already, _false_ otherwise
 */
static bool make_subdirs(std::string &p, const std::vector<std::string> &ss) {
  std::vector<std::string> tmp_path_components{ss};
  tmp_path_components.emplace(std::cbegin(tmp_path_components), p);

  const auto path{Glib::build_filename(tmp_path_components)};

  if (0 == g_mkdir_with_parents(path.c_str(), 0700)) {
    p = path;
    return true;
  }

  return false;
}
