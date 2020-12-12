/*
** Copyright (c) 2020, Xin YUAN, courses of Zhejiang University
** All rights reserved.
**
** This program is free software; you can redistribute it and/or
** modify it under the terms of the 2-Clause BSD License.
**
** Author contact information:
**   yxxinyuan@zju.edu.cn
**
*/

/*
This file contains classes for GTK+ widget.
*/

////////////////////////////////////////////////////////////////////////////////
// internal header
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////

//internal

// _os_widget

class _os_widget
{
public:
	explicit _os_widget(GtkWidget* w = NULL) noexcept : m_w(w)
	{
	}
	_os_widget(const _os_widget& src) noexcept : m_w(src.m_w)
	{
	}
	_os_widget& operator=(GtkWidget* w) noexcept
	{
		m_w = w;
		return *this;
	}
	_os_widget& operator=(const _os_widget& src) noexcept
	{
		m_w = src.m_w;
		return *this;
	}

	GtkWidget* GetHandle() const noexcept
	{
		return m_w;
	}
	bool IsValid() const noexcept
	{
		return m_w != NULL;
	}

	void Destroy() noexcept
	{
		if ( m_w != NULL ) {
			::gtk_widget_destroy(m_w);
			m_w = NULL;
		}
	}
	gboolean IsInDestruction() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_in_destrcution(m_w);
	}
	void Show() noexcept
	{
		assert( IsValid() );
		::gtk_widget_show(m_w);
	}
	void Hide() noexcept
	{
		assert( IsValid() );
		::gtk_widget_hide(m_w);
	}
	void ShowAll() noexcept
	{
		assert( IsValid() );
		::gtk_widget_show_all(m_w);
	}
	void Draw(cairo_t* cr) noexcept
	{
		assert( IsValid() );
		::gtk_widget_draw(m_w, cr);
	}
	void QueueDraw() noexcept
	{
		assert( IsValid() );
		::gtk_widget_queue_draw(m_w);
	}
	GdkFrameClock* GetFrameClock() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_frame_clock(m_w);
	}
	gint GetScaleFactor() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_scale_factor(m_w);
	}
	guint AddTickCallback(GtkTickCallback callback, gpointer user_data, GDestroyNotify notify) noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_add_tick_callback(m_w, callback, user_data, notify);
	}
	void RemoveTickCallback(guint id) noexcept
	{
		assert( IsValid() );
		::gtk_widget_remove_tick_callback(m_w, id);
	}
	void SizeAllocate(GtkAllocation* allocation) noexcept
	{
		assert( IsValid() );
		::gtk_widget_size_allocate(m_w, allocation);
	}
	void SizeAllocateWithBaseline(GtkAllocation* allocation, gint baseline) noexcept
	{
		assert( IsValid() );
		::gtk_widget_size_allocate_with_baseline(m_w, allocation, baseline);
	}
	void AddAccelerator(const gchar* accel_signal, GtkAccelGroup* accel_group, guint accel_key, GdkModifierType accel_mods, GtkAccelFlags accel_flags) noexcept
	{
		assert( IsValid() );
		::gtk_widget_add_accelerator(m_w, accel_signal, accel_group, accel_key, accel_mods, accel_flags);
	}
	gboolean RemoveAccelerator(GtkAccelGroup* accel_group, guint accel_key, GdkModifierType accel_mods) noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_remove_accelerator(m_w, accel_group, accel_key, accel_mods);
	}
	void SetAccelPath(const gchar* accel_path, GtkAccelGroup* accel_group) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_accel_path(m_w, accel_path, accel_group);
	}
	GList* ListAccelClosures() noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_list_accel_closures(m_w);
	}
	gboolean CanActivateAccel(guint signal_id) const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_can_activate_accel(m_w, signal_id);
	}
	gboolean Activate() noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_activate(m_w);
	}
	gboolean Intersect(const GtkRectangle* area, GtkRectangle* intersection) noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_intersect(m_w, area, intersection);
	}
	gboolean IsFocus() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_is_focus(m_w);
	}
	void GrabFocus() noexcept
	{
		assert( IsValid() );
		::gtk_widget_grab_focus(m_w);
	}
	void GrabDefault() noexcept
	{
		assert( IsValid() );
		::gtk_widget_grab_default(m_w);
	}
	void SetName(const gchar* name) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_name(m_w, name);
	}
	const gchar* GetName() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_name(m_w);
	}
	void SetSensitive(gboolean sensitive) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_sensitive(m_w, sensitive);
	}
	void SetParentWindow(GdkWindow* parent_window) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_parent_window(m_w, parent_window);
	}
	GdkWindow* GetParentWindow() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_parent_window(m_w);
	}
	void SetEvents(gint events) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_events(m_w, events);
	}
	gint GetEvents() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_events(m_w);
	}
	void AddEvents(gint events) noexcept
	{
		assert( IsValid() );
		::gtk_widget_add_events(m_w, events);
	}
	void SetDeviceEvents(GdkDevice* device, GdkEventMask events) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_device_events(m_w, device, events);
	}
	GdkEventMask GetDeviceEvents(GdkDevice* device) const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_device_events(m_w, device);
	}
	void AddDeviceEvents(GdkDevice* device, GdkEventMask events) noexcept
	{
		assert( IsValid() );
		::gtk_widget_add_device_events(m_w, device, events);
	}
	void SetDeviceEnabled(GdkDevice* device, gboolean enabled) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_device_enabled(m_w, device, enabled);
	}
	gboolean GetDeviceEnabled(GdkDevice* device) const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_device_enabled(m_w, device);
	}
	GtkWidget* GetTopLevel() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_toplevel(m_w);
	}
	GtkWidget* GetAncestor(GType widget_type) const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_ancestor(m_w, widget_type);
	}
	GdkVisual* GetVisual() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_visual(m_w);
	}
	void SetVisual(GdkVisual* visual) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_visual(m_w, visual);
	}
	gboolean IsAncestor(GtkWidget* ancestor) const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_is_ancestor(m_w, ancestor);
	}
	gboolean TranslateCoordinates(GtkWidget* dest_widget, gint src_x, gint src_y, gint* dest_x, gint* dest_y) const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_translate_coordinates(m_w, dest_widget, src_x, src_y, dest_x, dest_y);
	}
	void SetDirection(GtkTextDirection dir) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_direction(m_w, dir);
	}
	GtkTextDirection GetDirection() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_direction(m_w);
	}
	static void SetDefaultDirection(GtkTextDirection dir) noexcept
	{
		::gtk_widget_set_default_direction(dir);
	}
	static GtkTextDirection GetDefaultDirection() noexcept
	{
		return ::gtk_widget_get_default_direction();
	}
	void ShapeCombineRegion(cairo_region_t* region) noexcept
	{
		assert( IsValid() );
		::gtk_widget_shape_combine_region(m_w, region);
	}
	void InputShapeCombineRegion(cairo_region_t* region) noexcept
	{
		assert( IsValid() );
		::gtk_widget_input_shape_combine_region(m_w, region);
	}
	PangoContext* CreatePangoContext() noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_create_pango_context(m_w);
	}
	PangoContext* GetPangoContext() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_pango_context(m_w);
	}
	void SetFontOptions(const cairo_font_options_t* options) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_font_options(m_w, options);
	}
	const cairo_font_options_t* GetFontOptions() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_font_options(m_w);
	}
	void SetFontMap(PangoFontMap* font_map) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_font_map(m_w, font_map);
	}
	PangoFontMap* GetFontMap() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_font_map(m_w);
	}
	PangoLayout* CreatePangoLayout(const gchar* text) noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_create_pango_layout(m_w, text);
	}
	void QueueDrawArea(gint x, gint y, gint width, gint height) noexcept
	{
		assert( IsValid() );
		::gtk_widget_queue_draw_area(m_w, x, y, width, height);
	}
	void QueueDrawRegion(const cairo_region_t* region) noexcept
	{
		assert( IsValid() );
		::gtk_widget_queue_draw_region(m_w, region);
	}
	void SetAppPaintable(gboolean app_paintable) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_app_paintable(m_w, app_paintable);
	}
	void SetRedrawOnAllocate(gboolean redraw_on_allocate) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_redraw_on_allocate(m_w, redraw_on_allocate);
	}
	gboolean MnemonicActivate(gboolean group_cycling) noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_mnemonic_activate(m_w, group_cycling);
	}
	void StyleGetProerty(const gchar* property_name, GValue* value) const noexcept
	{
		assert( IsValid() );
		::gtk_widget_style_get_property(m_w, property_name, value);
	}
	AtkObject* GetAccessible() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_acessible(m_w);
	}
	void ChildNotify(const gchar* child_property) noexcept
	{
		assert( IsValid() );
		::gtk_widget_child_notify(m_w, child_property);
	}
	void FreezeChildNotify() noexcept
	{
		assert( IsValid() );
		::gtk_widget_freeze_child_notify(m_w);
	}
	GtkWidget* GetParnet() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_parent(m_w);
	}
	GtkSettings* GetSettings() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_settings(m_w);
	}
	GtkClipboard* GetClipboard(GdkAtom selection) const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_clipboard(m_w, selection);
	}
	GdkDisplay* GetDisplay() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_display(m_w);
	}
	GdkScreen* GetScreen() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_screen(m_w);
	}
	gboolean HasScreen() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_has_screen(m_w);
	}
	void GetSizeRequest(gint* width, gint* height) const noexcept
	{
		assert( IsValid() );
		::gtk_widget_get_size_request(m_w, width, height);
	}
	void SetSizeRequest(gint width, gint height) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_size_request(m_w, width, height);
	}
	void ThawChildNotify() noexcept
	{
		assert( IsValid() );
		::gtk_widget_thaw_child_notify(m_w);
	}
	void SetNoShowAll(gboolean no_show_all) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_no_show_all(m_w, no_show_all);
	}
	gboolean GetNoShowAll() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_no_show_all(m_w);
	}
	GList* ListMnemonicLabels() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_list_mnemonic_labels(m_w);
	}
	void AddMnemonicLabel(GtkWidget* label) noexcept
	{
		assert( IsValid() );
		::gtk_widget_add_mnemonic_label(m_w, label);
	}
	void RemoveMnemonicLabel(GtkWidget* label) noexcept
	{
		assert( IsValid() );
		::gtk_widget_remove_mnemonic_label(m_w, label);
	}
	void ErrorBell() noexcept
	{
		assert( IsValid() );
		::gtk_widget_error_bell(m_w);
	}
	gboolean KeynavFailed(GtkDirectionType direction) noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_keynav_failed(m_w, direction);
	}
	gchar* GetTooltipMarkup() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_tooltip_markup(m_w);
	}
	void SetTooltipMarkup(const gchar* markup) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_tooltip_markup(m_w, markup);
	}
	gchar* GetTooltipText() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_tooltip_text(m_w);
	}
	void SetTooltipText(const gchar* text) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_tooltip_text(m_w, text);
	}
	GtkWindow* GetTooltipWindow() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_tooltip_window(m_w);
	}
	void SetTooltipWindow(GtkWindow* custom_window) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_tooltip_window(m_w, custom_window);
	}
	gboolean GetHasTooltip() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_has_tooltip(m_w);
	}
	void SetHasTooltip(gboolean has_tooltip) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_has_tooltip(m_w, has_tooltip);
	}
	void TriggerTooltipQuery() noexcept
	{
		assert( IsValid() );
		::gtk_widget_trigger_tooltip_query(m_w);
	}
	GdkWindow* GetWindow() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_window(m_w);
	}
	void RegisterWindow(GdkWindow* window) noexcept
	{
		assert( IsValid() );
		::gtk_widget_register_window(m_w, window);
	}
	void UnregisterWindow(GdkWindow* window) noexcept
	{
		assert( IsValid() );
		::gtk_widget_unregister_window(m_w, window);
	}
	int GetAllocatedWidth() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_allocated_width(m_w);
	}
	int GetAllocatedHeight() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_allocated_height(m_w);
	}
	void GetAllocation(GtkAllocation* allocation) const noexcept
	{
		assert( IsValid() );
		::gtk_widget_get_allocation(m_w, allocation);
	}
	void SetAllocation(const GtkAllocation* allocation) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_allocation(m_w, allocation);
	}
	int GetAllocatedBaseline() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_allocated_baseline(m_w);
	}
	void GetAllocatedSize(GtkAllocation* allocation, int* baseline) const noexcept
	{
		assert( IsValid() );
		::gtk_widget_get_allocated_size(m_w, allocation, baseline);
	}
	void GetClip(GtkAllocation* clip) const noexcept
	{
		assert( IsValid() );
		::gtk_widget_get_clip(m_w, clip);
	}
	void SetClip(const GtkAllocation* clip) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_clip(m_w, clip);
	}
	gboolean GetAppPaintable() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_app_paintable(m_w);
	}
	gboolean GetCanDefault() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_can_default(m_w);
	}
	void SetCanDefault(gboolean can_default) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_can_default(m_w, can_default);
	}
	gboolean GetCanFocus() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_can_focus(m_w);
	}
	void SetCanFocus(gboolean can_focus) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_can_focus(m_w, can_focus);
	}
	gboolean GetFocusOnClick() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_focus_on_click(m_w);
	}
	void SetFocusOnClick(gboolean focus_on_click) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_focus_on_click(m_w, focus_on_click);
	}
	gboolean GetHasWindow() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_has_window(m_w);
	}
	gboolean GetSensitive() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_sensitive(m_w);
	}
	gboolean IsSensitive() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_is_sensitive(m_w);
	}
	gboolean GetVisible() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_visible(m_w);
	}
	gboolean IsVisible() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_is_visible(m_w);
	}
	void SetVisible(gboolean visible) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_visible(m_w, visible);
	}
	GtkStateFlags GetStateFlags() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_state_flags(m_w);
	}
	gboolean HasDefault() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_has_default(m_w);
	}
	gboolean HasFocus() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_has_focus(m_w);
	}
	gboolean HasVisibleFocus() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_has_visible_focus(m_w);
	}
	gboolean HasGrab() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_has_grab(m_w);
	}
	gboolean IsDrawable() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_is_drawable(m_w);
	}
	gboolean IsTopLevel() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_is_toplevel(m_w);
	}
	void SetWindow(GdkWindow* window) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_window(m_w, window);
	}
	void SetReceivesDefault(gboolean receives_default) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_receives_default(m_w, receives_default);
	}
	gboolean GetReceivesDefault() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_receives_default(m_w);
	}
	void SetSupportMultidevice(gboolean support_multidevice) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_support_multidevice(m_w, support_multidevice);
	}
	gboolean GetSupportMultidevice() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_support_multidevice(m_w);
	}
	gboolean GetRealized() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_realized(m_w);
	}
	gboolean GetMapped() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_mapped(m_w);
	}
	gboolean DeviceIsShadowed(GdkDevice* device) const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_device_is_shadowed(m_w, device);
	}
	GdkModifierType GetModifierMask(GdkModifierIntent intent) const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_modifier_mask(m_w, intent);
	}
	void InsertActionGroup(const gchar* name, GActionGroup* group) noexcept
	{
		assert( IsValid() );
		::gtk_widget_insert_action_group(m_w, name, group);
	}
	double GetOpacity() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_opacity(m_w);
	}
	void SetOpacity(double opacity) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_opacity(m_w, opacity);
	}
	const gchar** ListActionPrefixes() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_list_action_prefixes(m_w);
	}
	GActionGroup* GetActionGroup(const gchar* prefix) const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_action_group(m_w, prefix);
	}
	GtkWidgetPath* GetPath() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_path(m_w);
	}
	GtkStyleContext* GetStyleContext() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_style_context(m_w);
	}
	void ResetStyle() noexcept
	{
		assert( IsValid() );
		::gtk_widget_reset_style(m_w);
	}
	void GetPreferredHeight(gint* minimum_height, gint* natural_height) const noexcept
	{
		assert( IsValid() );
		::gtk_widget_get_preferred_height(m_w, minimum_height, natural_height);
	}
	void GetPreferredWidth(gint* minimum_width, gint* natural_width) const noexcept
	{
		assert( IsValid() );
		::gtk_widget_get_preferred_width(m_w, minimum_width, natural_width);
	}
	void GetPreferredHeightForWidth(gint width, gint* minimum_height, gint* natural_height) const noexcept
	{
		assert( IsValid() );
		::gtk_widget_get_preferred_height_for_width(m_w, width, minimum_height, natural_height);
	}
	void GetPreferredWidthForHeight(gint height, gint* minimum_width, gint* natural_width) const noexcept
	{
		assert( IsValid() );
		::gtk_widget_get_preferred_width_for_height(m_w, height, minimum_width, natural_width);
	}
	void GetPreferredHeightAndBaselineForWidth(gint width, gint* minimum_height, gint* natural_height, gint* minimum_baseline, gint* natural_baseline) const noexcept
	{
		assert( IsValid() );
		::gtk_widget_get_preferred_height_and_baseline_for_width(m_w, width, minimum_height, natural_height, minimum_baseline, natural_baseline);
	}
	GtkSizeRequestMode GetRequestMode() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_request_mode(m_w);
	}
	void GetPreferredSize(GtkRequisition* minimum_size, GtkRequisition* natural_size) const noexcept
	{
		assert( IsValid() );
		::gtk_widget_get_preferred_size(m_w, minimum_size, natural_size);
	}
	GtkAlign GetHalign() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_halign(m_w);
	}
	void SetHalign(GtkAlign align) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_halign(m_w, align);
	}
	GtkAlign GetValign() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_valign(m_w);
	}
	GtkAlign GetValignWithBaseline() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_valign_with_baseline(m_w);
	}
	void SetValign(GtkAlign align) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_valign(m_w, align);
	}
	gint GetMarginStart() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_margin_start(m_w);
	}
	void SetMarginStart(gint margin) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_margin_start(m_w, margin);
	}
	gint GetMarginEnd() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_margin_end(m_w);
	}
	void SetMarginEnd(gint margin) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_margin_end(m_w, margin);
	}
	gint GetMarginTop() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_margin_top(m_w);
	}
	void SetMarginTop(gint margin) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_margin_top(m_w, margin);
	}
	gint GetMarginBottom() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_margin_bottom(m_w);
	}
	void SetMarginBottom(gint margin) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_margin_bottom(m_w, margin);
	}
	gboolean GetHexpand() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_hexpand(m_w);
	}
	void SetHexpand(gboolean expand) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_hexpand(m_w, expand);
	}
	gboolean GetHexpandSet() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_hexpand_set(m_w);
	}
	void SetHexpandSet(gboolean set) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_hexpand_set(m_w, set);
	}
	gboolean GetVexpand() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_vexpand(m_w);
	}
	void SetVexpand(gboolean expand) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_vexpand(m_w, expand);
	}
	gboolean GetVexpandSet() const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_get_vexpand_set(m_w);
	}
	void SetVexpandSet(gboolean set) noexcept
	{
		assert( IsValid() );
		::gtk_widget_set_vexpand_set(m_w, set);
	}
	void QueueComputeExpand() noexcept
	{
		assert( IsValid() );
		::gtk_widget_queue_compute_expand(m_w);
	}
	gboolean ComputeExpand(GtkOrientation orientation) const noexcept
	{
		assert( IsValid() );
		return ::gtk_widget_compute_expand(m_w, orientation);
	}

private:
	GtkWidget* m_w;
};

////////////////////////////////////////////////////////////////////////////////
