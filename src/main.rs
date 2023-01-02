use std::sync::Arc;

use druid::{AppLauncher, WindowDesc, Widget, PlatformError, Data, WidgetExt, Lens, Env, FontDescriptor, FontFamily};
use druid::widget::{Label, Flex, List, Padding, TextBox, CrossAxisAlignment, MainAxisAlignment};

#[derive(Clone, Data, Lens)]
struct Test {
    pub name: String
}

#[derive(Clone, Data, Lens)]
struct AppState {
    pub tests: Arc<Vec<Test>>,
    pub output: String
}

fn build_ui() -> impl Widget<AppState> {
    Padding::new(10.0,
        Flex::row()
            .cross_axis_alignment(CrossAxisAlignment::Start)
            .main_axis_alignment(MainAxisAlignment::Center)
            .with_child(
                Flex::column()
                    .main_axis_alignment(MainAxisAlignment::Start)
                    .with_child(Label::new("Tests"))
                    .with_child(List::new(|| Label::new(|data: &Test, _env: &Env| data.name.clone())).lens(AppState::tests))
                )
            .with_default_spacer()
            .with_child(
                TextBox::multiline().with_font(FontDescriptor::new(FontFamily::MONOSPACE)).fix_width(300.0).height(500.0).expand_height().lens(AppState::output).disabled_if(|_, _| true)))
}

fn main() -> Result<(), PlatformError> {
    AppLauncher::with_window(WindowDesc::new(build_ui()).title("Diagonal")).launch(AppState { tests: Arc::new(Vec::new()), output: "Output goes here".into() })?;
    Ok(())
}
