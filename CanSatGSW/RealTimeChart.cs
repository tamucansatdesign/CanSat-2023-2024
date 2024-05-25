using System.Drawing;
using System.Windows.Forms;
using System.Windows.Forms.DataVisualization.Charting;

namespace CanSat_GSW
{
    public partial class RealTimeChart : UserControl
    {

        public RealTimeChart()
        {
            InitializeComponent();
        }

        public void setTitle(string title)
        {
            chart1.Titles[0].Text = title;
        }

        public void AddChart(string xLabel, string yLabel, string seriesName, SeriesChartType type)
        {
            string chartName = "chart" + chart1.ChartAreas.Count;
            chart1.ChartAreas.Add(chartName);
            chart1.ChartAreas[chartName].AxisX.Title = xLabel;
            chart1.ChartAreas[chartName].AxisX.TitleForeColor = Color.White;
            chart1.ChartAreas[chartName].AxisY.Title = yLabel;
            chart1.ChartAreas[chartName].AxisY.TitleForeColor = Color.White;
            chart1.ChartAreas[chartName].BackColor = Color.Black;
            chart1.ChartAreas[chartName].BorderColor = Color.White;

            chart1.ChartAreas[chartName].AxisX.LineColor = Color.White;
            chart1.ChartAreas[chartName].AxisX.MajorGrid.LineColor = Color.Gray;
            chart1.ChartAreas[chartName].AxisX.LabelStyle.ForeColor = Color.White;
            chart1.ChartAreas[chartName].AxisX.LabelStyle.Format = ".2";

            chart1.ChartAreas[chartName].AxisY.LineColor = Color.White;
            chart1.ChartAreas[chartName].AxisY.MajorGrid.LineColor = Color.Gray;
            chart1.ChartAreas[chartName].AxisY.LabelStyle.ForeColor = Color.White;
            chart1.ChartAreas[chartName].AxisX.LabelStyle.Format = ".2";

            chart1.Series.Add(seriesName);
            chart1.Series[seriesName].ChartArea = chartName;
            chart1.Series[seriesName].ChartType = type;
        }

        public void AddLineToChart(string chartName, string seriesName, SeriesChartType type)
        {
            chart1.Series.Add(seriesName);
            chart1.Series[seriesName].ChartArea = chartName;
            chart1.Series[seriesName].ChartType = type;
        }

        public void AddPoint<x_type, y_type>(x_type x, y_type y, string seriesName)
        {
            chart1.Series[seriesName].Points.AddXY(x, y);
            if (chart1.Series[seriesName].Points.Count > 20) chart1.Series[seriesName].Points.RemoveAt(0);
        }

        public void Clear()
        {
            foreach (Series series in chart1.Series)
            {
                series.Points.Clear();
            }
        }
    }
}
