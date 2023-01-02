use std::sync::Arc;

use druid::{AppLauncher, WindowDesc, Widget, PlatformError, Data, WidgetExt, Lens, Env, FontDescriptor, FontFamily};
use druid::widget::{Label, Flex, List, Padding, TextBox};

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
            .with_flex_child(
                Padding::new((10.0, 0.0), Flex::column()
                    .with_flex_child(Label::new("Tests"), 1.0)
                    .with_flex_child(List::new(|| Label::new(|data: &Test, _env: &Env| data.name.clone())).lens(AppState::tests), 1.0)
                ), 1.0)
            .with_flex_child(
                TextBox::multiline().with_font(FontDescriptor::new(FontFamily::MONOSPACE)).lens(AppState::output).disabled_if(|_, _| true).expand_height().expand_width(), 1.0))
}

fn main() -> Result<(), PlatformError> {
    AppLauncher::with_window(WindowDesc::new(build_ui())).launch(AppState { tests: Arc::new(Vec::new()), output: "Output goes here".into() })?;
    Ok(())
}
