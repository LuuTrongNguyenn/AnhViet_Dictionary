using System;
using System.Drawing;
using System.Windows.Forms;
using Microsoft.Web.WebView2.WinForms;
namespace LexiTrie;
internal static class Program {
 [STAThread] static void Main() {
  ApplicationConfiguration.Initialize();
  using var form = new Form { Text="LexiTrie — Smart English–Vietnamese Dictionary", Width=1400, Height=900, MinimumSize=new Size(900,650), StartPosition=FormStartPosition.CenterScreen, BackColor=Color.FromArgb(9,14,28) };
  using var web = new WebView2 { Dock=DockStyle.Fill, DefaultBackgroundColor=Color.FromArgb(9,14,28) };
  form.Controls.Add(web);
  form.Shown += async (_,_)=>{ try { await web.EnsureCoreWebView2Async(); web.CoreWebView2.Settings.AreDevToolsEnabled=true; web.CoreWebView2.Navigate("http://127.0.0.1:18765/"); } catch(Exception ex){ MessageBox.Show("Không thể khởi động WebView2. Hãy cài Microsoft Edge WebView2 Runtime.\n"+ex.Message); } };
  Application.Run(form);
 }
}
