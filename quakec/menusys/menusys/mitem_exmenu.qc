/***************************************************************************
fullscreen exclusive menu
you should only have ONE of these visible at once.
interactable - basically just a container for the items in the menu, but also handles killing them+itself when the user presses escape.
will keep stealing focus from the desktop, so you won't be able to play while one of these is active.
will not steal focus from siblings. this means console slideouts or whatever are still usable.

these items will automatically be added to the desktop/workspace thing
Call it.item_remove() to pop the menu.
Regular items can be added to the menu by first spawning them, then calling menu_additem to actually add it to the desired menu in the right place.
*/

class mitem_exmenu : mitem_frame
{
	virtual float(vector pos, float scan, float char, float down) item_keypress =
	{
		local float ret = super::item_keypress(pos, scan, char, down);
		if (!ret && down)
		{
			ret = TRUE;
			if (scan == K_MOUSE2 || scan == K_ESCAPE)
			{
				localcmd(strcat(item_command, "\n"));	//console command to exec if someone clicks the close button.
				item_remove();
			}
			else if (scan == K_UPARROW && down)
				this.item_focuschange(menu_nextitem(this, item_kactivechild?item_kactivechild:item_mactivechild), IF_KFOCUSED);
			else if (scan == K_DOWNARROW && down)
				this.item_focuschange(menu_previousitem(this, item_kactivechild?item_kactivechild:item_mactivechild), IF_KFOCUSED);
			else if (scan >= K_F1 && scan <= K_F12)	//allow f1-f12 to work, but every other button event gets canceled.
				ret = FALSE;
		}
		return ret;
	};
	virtual void(vector pos) item_draw =
	{
		if (ui.mgrabs == item_parent || ui.kgrabs == item_parent)	//if our parent has grabs, steal it instead, this means you can select the console, but the game never gets focus
		{
			if (item_flags & IF_SELECTABLE)
			{
				ui.mgrabs = __NULL__;
				ui.kgrabs = __NULL__;
				item_parent.item_focuschange(this, IF_KFOCUSED);
			}
		}
		super::item_draw(pos);
	};

	//same as mitem, but does not propogate to parent.
	virtual string(string key) get =
	{
		return cvar_string(key);
	};
	virtual void(string key, string newval) set =
	{
		cvar_set(key, newval);
	};
};